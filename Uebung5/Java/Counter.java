package aufgabe2;
/*
  Diese Klasse zählt die gemeinsame Variable in mit der Methode increment() hoch
*/
public class Counter {

  //erhöht die globale variable in um eins, monitorsynchronisiert
  public synchronized void increment() {
    long local = ThreadTest.in;
    local++;
    ThreadTest.in = local;
  }

  //returned den aktuelle wert von in als string
  public String print() {
    return ""+ThreadTest.in;
  }

}
