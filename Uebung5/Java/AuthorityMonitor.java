import java.util.LinkedList;
public synchronized class AuthorityMonitor {

  //Queue for applicants (Waiting room)
  private LinkedList<Applicant> queue = new LinkedList<Applicant>();
  private int maxSize = 10;

  //Request for the official
  private Request r = null;

  //Wait for a request
  Request check() {
    while (r == null) {   //Sleep ("Busy" waiting)
      //Check for applicants
      if (queue.size() > 0)
        return queue.remove().dropReq();
    }
    return r;
  }

  //Applicant visits authority
  void visit(Applicant a) {
    //Official sleeping?
    if (r == null) {
      //enter official's room, drop request and leave
      r = a.dropReq();
    } else {
      //official is working rn
      if (queue.size() < maxSize)   //== waiting room not full
        queue.add(a);
    }
  }
}
