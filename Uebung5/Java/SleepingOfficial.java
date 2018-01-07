
public class SleepingOfficial {

  //Produce a new Request
  public static Request produceRequest() {
    return new Request((int) (Math.random() * Integer.MAX_VALUE));
  }

  //Main function
  public static void main(String[] args) throws InterruptedException {
    AuthorityMonitor monitor = new AuthorityMonitor();
    Official off = new Official(monitor);
    off.start();

    while (true) {
      Request r = produceRequest();
      //Sleep a random amount of time 1s - 5s
      int s = (int) (Math.random() * 5) + 1;
      System.out.println(s);
      Thread.sleep(s * 1000);
      Applicant app = new Applicant(r, monitor);
      app.start();
    }
  }

}
