
/*
  Diese Klasse zählt die gemeinsame Variable in mit der Methode increment() hoch
*/
public class Counter {

  //globale variable
  private static long in = 0;

  public synchronized void increment() {
    long local = in;
    local++;
    in = local;
  }

  public String print() {
    return ""+in;
  }

}
