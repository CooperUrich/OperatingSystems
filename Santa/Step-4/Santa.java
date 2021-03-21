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
				for(Elf elf: s.inTrouble) {
					if (elf.getState() == Elf.ElfState.AT_SANTAS_DOOR)
						elf.setState(Elf.ElfState.WORKING);
				}
				s.inTrouble.clear();
			
				// Fix the elves problems at the door that are in trouble
				
				// Clear the number of elves at santas door
				s.inTrouble.clear();
				// Indicate that the elves are back from the door, meaning the next three elves can go
				s.elvesBackFromDoor = false;
				// Santa goes to sleep after each elf has gone back to work
				this.state = SantaState.SLEEPING;
				break;
			case WOKEN_UP_BY_REINDEER: 
				// FIXME: assemble the reindeer to the sleigh then change state to ready 
				break;
			case READY_FOR_CHRISTMAS: // nothing more to be done
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
