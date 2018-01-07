
public class Official extends Thread {

  private Request r;
  private AuthorityMonitor monitor;

  public Official(AuthorityMonitor monitor) {
    this.monitor = monitor;
    r = null;
  }

  //Process a request
  private void process() {
    if (r == null)
      return;

    //read the int from r
    int i = r.stuff;
    System.out.println("Processed request with int stuff="+i);
  }

  public void run() {
    while(true) {
      r = monitor.check();
      process();
    }
  }
}
