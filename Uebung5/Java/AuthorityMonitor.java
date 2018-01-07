import java.util.LinkedList;
public class AuthorityMonitor {

  //Queue for applicants (Waiting room)
  private LinkedList<Applicant> queue = new LinkedList<Applicant>();
  private int maxSize = 10;

  //Request for the official
  private Request r = null;

  //Wait for a request
  public Request check() {
    System.out.println("Check()...");
    while (r == null) {
      //Check for applicants
      if (queue.size() > 0)
        return queue.remove().dropReq();
    }
    return r;
  }

  //Applicant visits authority
  public void visit(Applicant a) {
    System.out.println("Visit()...");
    //Official sleeping?
    if (r == null) {
      //enter official's room, drop request and leave
      r = a.dropReq();
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
