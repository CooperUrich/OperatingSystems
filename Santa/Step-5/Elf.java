import java.util.Random;

public class Elf implements Runnable {

	enum ElfState {
		WORKING, TROUBLE, AT_SANTAS_DOOR
	};

	private ElfState state;
	/**
	 * The number associated with the Elf
	 */
	private int number;
	
	private Random rand = new Random();
	private SantaScenario scenario;
	private boolean running = true;
	public boolean waitingToLeaveForSanta;

	public Elf(int number, SantaScenario scenario) {
		this.number = number;
		this.scenario = scenario;
		this.state = ElfState.WORKING;
	}
	// 
	public void waitingToLeave(){
		waitingToLeaveForSanta = false;
	}
	public void elfCanLeave(){
		waitingToLeaveForSanta = true;
	}
	public boolean getStatus(){
		return waitingToLeaveForSanta;
	}
	// Gets current state
	public ElfState getState() {
		return state;
	}
	// Terminates the current elf thread
	public void timetodie(){
		this.running = false;
	}
	/**
	 * Santa might call this function to fix the trouble
	 * @param state
	 */
	public void setState(ElfState state) {
		this.state = state;
	}


	@Override
	public void run() {
		while (running) {
      // wait a day
  		try {
  			Thread.sleep(100);
  		} catch (InterruptedException e) {
  			// TODO Auto-generated catch block
  			e.printStackTrace();
  		}
			switch (state) {
			case WORKING: {
				// at each day, there is a 1% chance that an elf runs into
				// trouble.
				waitingToLeave();
				if (rand.nextDouble() < 0.01) {
					state = ElfState.TROUBLE;
				}
				break;
			}
			// Only 3 elves can be at santas door
			case TROUBLE:
			if (!scenario.getReadyForChristmas()){
				try {
					scenario.elfInTrouble.acquire();
					if (getStatus() == false){
						scenario.inTrouble.add(this);
						elfCanLeave();
					}
					scenario.elfInTrouble.release();

				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
				break;
			case AT_SANTAS_DOOR:
				// When an elf is at santas door, alert santa that elves are at the door 
				// scenario.santa.WokenUpByElves();
				break;
			}
		}
	}

	/**
	 * Report about my state
	 */
	public void report() {
		// Styling purposes so everything lines up
		if (number == 10){
			System.out.println("Elf " + number + ": " + state);
		}
		else {
			System.out.println("Elf " + number + " : " + state);
		}
	}

}
