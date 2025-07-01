//> using scala "3.3.1"
//> using dep "io.circe::circe-core:0.14.14"
//> using dep "io.circe::circe-generic:0.14.14"
//> using dep "io.circe::circe-parser:0.14.14"

import io.circe._
import io.circe.generic.auto._
import io.circe.parser.decode
import io.circe.syntax._
import scala.sys.process._
import java.io.{File, PrintWriter}
import scala.io.Source
import scala.util.{Try, Using}

// --- Modele danych dla JSON ---
sealed trait Command
case class AddVehicle(vehicleId: String, startRoad: String, endRoad: String) extends Command
case class Step() extends Command

implicit val commandDecoder: Decoder[Command] = (c: HCursor) =>
  c.downField("type").as[String].flatMap {
    case "addVehicle" => c.as[AddVehicle]
    case "step"       => Right(Step())
    case other        => Left(DecodingFailure(s"Unknown command type: $other", c.history))
  }

case class InputCommands(commands: List[Command])
case class StepStatus(leftVehicles: List[String])
case class SimulationOutput(stepStatuses: List[StepStatus])

// --- Główny obiekt symulacji ---
// Obiekt singleton, grupujący całą logikę wykonawczą skryptu.
object SimulationRunner {
  val C_EXECUTABLE = if (System.getProperty("os.name").startsWith("Windows")) "./simulation.exe" else "./simulation"
  val STATE_FILE = "simulation.state"

  /**
   * Wykonuje zewnętrzną komendę (program w C) i przechwytuje jej standardowe wyjście (stdout).
   * @param args Sekwencja argumentów do przekazania do programu C.
   * @return Wynik działania (stdout) opakowany w Try, aby obsłużyć ewentualne błędy.
   */
  def runCCommand(args: Seq[String]): Try[String] = Try {
    Process(args).!!
  }

  /** Główny punkt startowy logiki skryptu. */
  def main(args: Array[String]): Unit = {
    // 1. Walidacja argumentów wejściowych
    if (args.length != 2) {
      println("Sposób użycia: scala-cli simulation.sc -- <plik_wejsciowy.json> <plik_wyjsciowy.json>")
      sys.exit(1)
    }

    val inputFile = args(0)
    val outputFile = args(1)

    // 2. Przygotowanie środowiska - usunięcie starego pliku stanu, aby każda symulacja była czysta.
    new File(STATE_FILE).delete()

    // 3. Wczytanie i sparsowanie pliku JSON z komendami.
    // 'Using.resource' zapewnia, że plik zostanie bezpiecznie zamknięty.
    val commandsJson = Using.resource(Source.fromFile(inputFile))(_.mkString)
    val commands = decode[InputCommands](commandsJson) match {
      case Right(parsed) => parsed.commands
      case Left(error)   =>
        println(s"Błąd parsowania pliku wejściowego: $error")
        sys.exit(1)
    }

    println(s"Rozpoczynanie symulacji, załadowano ${commands.length} komend.")
    
    // 4. Wykonanie komend w pętli.
    // 'flatMap' jest użyty, ponieważ tylko komenda 'step' generuje wynik (StepStatus),
    // a 'addVehicle' nie generuje nic (None), więc zostanie odfiltrowana.
    val stepStatuses = commands.flatMap {
      case AddVehicle(id, start, end) =>
        println(s"  -> addVehicle: $id ($start -> $end)")
        runCCommand(Seq(C_EXECUTABLE, "addVehicle", id, start.toUpperCase, end.toUpperCase))
        None

      case Step() =>
        println("  -> step")
        val output = runCCommand(Seq(C_EXECUTABLE, "step")).getOrElse("")
        val leftVehicles = if (output.trim.isEmpty) List.empty else output.trim.split("\\R").toList
        Some(StepStatus(leftVehicles))
    }

    // 5. Sformatowanie wyników do JSON i zapis do pliku.
    val simulationOutput = SimulationOutput(stepStatuses)
    Using.resource(new PrintWriter(outputFile)) { writer =>
      writer.write(simulationOutput.asJson.spaces2)
    }

    println(s"Symulacja zakończona. Wyniki zapisano w '$outputFile'.")

    // 6. Sprzątanie po symulacji.
    new File(STATE_FILE).delete()
  }
}

// Wywołanie głównej logiki. To jest punkt startowy całego skryptu.
SimulationRunner.main(args)