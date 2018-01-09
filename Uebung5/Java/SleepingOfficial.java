package aufgabe4;

public class SleepingOfficial {

  //Produce a new Request
  public static Request produceRequest() {
    return new Request((int) (Math.random() * 10));
  }

  //Main function
  public static void main(String[] args) throws InterruptedException {
    AuthorityMonitor monitor = new AuthorityMonitor();
    Official off = new Official(monitor);
    off.start();

    long startingTime = System.currentTimeMillis();

    while (true) {
      Request r = produceRequest();

      if (System.currentTimeMillis() - startingTime <= 2000 ) {
        Applicant app = new Applicant(r, monitor);
        Thread.sleep(0);
        app.start();
      }
      else if (System.currentTimeMillis() - startingTime >= 4000) {
        startingTime = System.currentTimeMillis();
      }

    }

  }


}
