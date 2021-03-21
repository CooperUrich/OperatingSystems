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
	// Kills the thread
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
			case SLEEPING: // if sleeping, continue to sleep
				break;

			// if santa is woken by elves
			case WOKEN_UP_BY_ELVES: 
				// Cycle through each elf to see who is at the door
				for(Elf elf: s.elves) {
					// Get the State
					elfState = elf.getState();
					// If the state is AT_SANTAS_DOOR...
					if (elfState == Elf.ElfState.AT_SANTAS_DOOR){
						// ...Send the elf back to work
						elf.setState(Elf.ElfState.WORKING);
						
					}
				}
				backToSleep();
	
				break;
			case WOKEN_UP_BY_REINDEER: 
				// FIXME: assemble the reindeer to the sleigh then change state to ready 
				break;
			case READY_FOR_CHRISTMAS: // nothing more to be done
				break;
			}
		}
	}
	// Changed the state to WOKEN_UP_BY_ELVES
	public void WokenUpByElves(){
		this.state = SantaState.WOKEN_UP_BY_ELVES;
	}
	// Puts santa back to sleep
	public void backToSleep(){
		this.state = SantaState.SLEEPING;
	}

	
	/**
	 * Report about my state
	 */
	public void report() {
		System.out.println("Santa : " + state);
	}
	
	
}
