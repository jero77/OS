
/*
  Testet die Klasse MyThread. Es werden mehrere Threads mit Race-Condition
  erzeugt. Synchronisation erfolgt über Monitore.
*/
public class ThreadTest {

  public static void main(String[] args) {

    //Anzahl Threads
    int num = 4;

    if (args.length != 1) {
      System.out.println("Erwartet wird eine pos. ganze Zahl als Komm.Param.");
      System.exit(-1);
    }

    long max = Long.parseLong(args[0]);

    MyThread[] thread = new MyThread[num];
    Counter count = new Counter();
    for (int i = 0; i < num; i++) {
      thread[i] = new MyThread(i, max, count);
      thread[i].start();
      System.out.println("Thread "+i+" gestartet");
    }

    for (int i = 0; i < num; i++) {
      try {
        thread[i].join();
      } catch (InterruptedException e) {

      }
    }

    System.out.println("Ergebnis in="+count.print());
  }


}
