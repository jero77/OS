import java.util.LinkedList;
public class AuthorityMonitor {

  //Queue for applicants (Waiting room)
  private LinkedList<Applicant> queue = new LinkedList<Applicant>();
  private int maxSize = 2000;

  //Request for the official
  private Request r = null;

  //Wait for a request
  public synchronized Request check() {
    Request ret;
    if (r == null && queue.size() <= 0) {
      try {
        System.out.println("zzzzz....zzzzz....");
        wait();
      }
      catch (InterruptedException e) {
        System.out.println(e.getMessage());
      }
    }

    if (r == null)
      r = queue.remove().dropReq();

    ret = r;
    r = null;
    return ret;
  }

  //Applicant visits authority
  public synchronized void visit(Applicant a) {

    //Official sleeping?
    if (r == null && queue.size() <= 0) {
      //enter official's room, drop request and leave
      r = a.dropReq();
      notify();
      System.out.println("Dropped Request!");
    } else {
      //official is working rn
      if (queue.size() < maxSize) {  //== waiting room not full
        queue.add(a);
        System.out.println("Waiting in the waiting room...");
      } else
        System.out.println("Leaving (waiting room full)!");
    }
  }
}
