import java.util.*;

public class SleepingOfficial {

  AuthorityMonitor monitor = new AuthorityMonitor();


  //Inner class for monitor
  class AuthorityMonitor {

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


  //Inner class for request
  class Request {
    int stuff;
    Request(int stuff) {
      this.stuff = stuff;
    }
  }


  //Inner class for Official
  class Official extends Thread {
    Request r;
    public void run() {
      while(true) {
        r = monitor.check();
        process(r);
      }
    }
  }


  //Inner class for applicant
  class Applicant extends Thread {
    Request r;
    Applicant(Request r) {
      this.r = r;
    }
    Request dropReq() {
      return r;
    }
    public void run() {
      monitor.visit(r);
    }
  }


  //Process a request
  private static void process(Request r) {
    //read the int from r
    int i = r.stuff;
    System.out.println("Processed request with int stuff="+i);
  }

  //Produce a new Request
  private static Request produceRequest() {
    return new Request( (int) Math.random() * Integer.MAX_VALUE);
  }



  //Main function
  public static void main(String[] args) {
    Official off = new Official();
    off.start();

    while (true) {
      Request r = produceRequest();
    }

  }



}
