
public class Applicant extends Thread {

  private Request r;
  private AuthorityMonitor monitor;

  public Applicant(Request r, AuthorityMonitor monitor) {
    this.r = r;
    this.monitor = monitor;
  }

  public Request dropReq() {
    return r;
  }

  public void run() {
    System.out.println("Visiting monitor...");
    monitor.visit(this);
  }
}
