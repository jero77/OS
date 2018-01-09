package aufgabe2;
/*
  Diese Klasse erbt von Thread und erlaubt es mehrere Threads parallel zu
  starten, die auf die gemeinsame Variable in der Klasse Counter zugreifen.
*/
public class MyThread extends Thread {

  //Klassenvariablen
  private int id;
  private long count_max;
  private Counter counter;

  //Konstruktor
  public MyThread (int id, long count_max, Counter counter) {
    this.id = id;
    this.count_max = count_max;
    this.counter = counter;
  }

  //Threadfunktion
  public void run() {

    //Erhöhe in count_max mal um eins
    for (long i = 0; i < count_max; i++ )
      counter.increment();

  }


}
