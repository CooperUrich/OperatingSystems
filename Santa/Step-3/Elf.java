import java.util.Random;
import java.nio.file.FileSystemAlreadyExistsException;
import java.util.ArrayList;

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
	public void waitingToLeave(){
		waitingToLeaveForSanta = false;
	}


	public ElfState getState() {
		return state;
	}
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
				if (rand.nextDouble() < 0.01) {
					state = ElfState.TROUBLE;
				}
				break;
			}
			// Only 3 elves can be at santas door
			case TROUBLE:
				// add this elf to the list of elves in trouble
				// If there are less than three elves at the door, and there are no
				// elves waiting to leave
				if (scenario.waiting.size() > 0){
					for (int i = 0; i < 3; i++){

					}
				}
				
				if (scenario.elvesBackFromDoor == false && scenario.inTrouble.size() < 3 && waitingToLeaveForSanta == false){
					// If the old elves are back from the door
					


					// This will be the third elf in trouble
					if (scenario.inTrouble.size() == 2){
						scenario.inTrouble.add(this);
						// Send the elves to santas door, back from door is false
						scenario.elvesBackFromDoor = true;
						waitingToLeaveForSanta = true;
						for (Elf elf : scenario.inTrouble){
							// no elves are waiting to go to santa, so new
							// elves can now be at the door
							// There is now no wait to leave for santa
							elf.waitingToLeave();
							// Send the elves to the door
							elf.setState(ElfState.AT_SANTAS_DOOR);
							
							
							
						}
					}
					// Not 
					if (scenario.inTrouble.size() < 2){
						// Add this elf to the inTrouble List
						scenario.inTrouble.add(this);
						waitingToLeaveForSanta = true;

					}

				}
				// state = ElfState.AT_SANTAS_DOOR;
				// scenario.setNumberOfElves();
				break;
			case AT_SANTAS_DOOR:
				// When an elf is at santas door, alert santa that elves are at the door 
				scenario.santa.WokenUpByElves();
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
