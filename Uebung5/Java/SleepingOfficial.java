
public class SleepingOfficial {

  //Produce a new Request
  public static Request produceRequest() {
    return new Request((int)Math.random() * Integer.MAX_VALUE);
  }

  //Main function
  public static void main(String[] args) {
    AuthorityMonitor monitor = new AuthorityMonitor();
    Official off = new Official(monitor);
    off.start();

    while (true) {
      Request r = produceRequest();
      Applicant app = new Applicant(r, monitor);
      app.start();
    }
  }

}
