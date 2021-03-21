//import com.sun.org.apache.xml.internal.security.utils.HelperNodeList;


public class Santa implements Runnable {

	enum SantaState {SLEEPING, READY_FOR_CHRISTMAS, WOKEN_UP_BY_ELVES, WOKEN_UP_BY_REINDEER};
	private SantaState state;
	public Elf.ElfState elfState;
	SantaScenario s;
	private boolean running = true;
	
	public Santa(SantaScenario scenario) {
		this.state = SantaState.SLEEPING;
		this.s = scenario;

	}
	// Stop running the thread
	public void timetodie(){
		running = false;
	}
	
	
	@Override
	public void run() {
		while(running) {
			// wait a day...
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			switch(state) {
			case SLEEPING: 
				break;
			case WOKEN_UP_BY_ELVES: 
				break;
			case WOKEN_UP_BY_REINDEER: 
				break;
			case READY_FOR_CHRISTMAS: 
				break;
			}
		}
	}
	public void WokenUpByElves(){
		this.state = SantaState.WOKEN_UP_BY_ELVES;
	}

	
	/**
	 * Report about my state
	 */
	public void report() {
		System.out.println("Santa : " + state);
	}
	
	
}
