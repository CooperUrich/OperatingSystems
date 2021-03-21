import java.util.ArrayList;
import java.util.Queue;
import java.util.List;
import java.util.concurrent.Semaphore;
import java.util.concurrent.ArrayBlockingQueue;

public class SantaScenario {
	public int numberOfElvesAtDoor = 0;
	public Santa santa;
	public List<Elf> elves;
	public List<Elf> inTrouble;
	public List<Elf> waiting;
	public boolean isDecember;
	public boolean elvesBackFromDoor;
	public Semaphore elfInTrouble;
	public Semaphore santaSemaphore;
	public Semaphore mutex;
	public Queue<Elf> semaphoreQueue;
	public Object semaphoreLock = new Object();

	public void setNumberOfElves(){
		this.numberOfElvesAtDoor = numberOfElvesAtDoor + 1;
	}
	public void setNumberOfElvesExact(int num){
		this.numberOfElvesAtDoor = num;
	}
	public int getNumberOfElves(){
		return numberOfElvesAtDoor;
	}
	public static void main(String args[]){
		SantaScenario scenario = new SantaScenario();
		scenario.isDecember = false;
		// create the participants
		// Santa
		scenario.santa = new Santa(scenario);
		Thread th = new Thread(scenario.santa);
		th.start();
		// The elves: in this case: 10
		scenario.elves = new ArrayList<>();
		for(int i = 0; i != 10; i++) {
			Elf elf = new Elf(i+1, scenario);
			scenario.elves.add(elf);
			th = new Thread(elf);
			th.start();
		}
		scenario.inTrouble = new ArrayList<>();
		// Semaphore that permits 3 elves and permits FIFO permits in the Queue
		scenario.elfInTrouble = new Semaphore(1, true);
		scenario.santaSemaphore = new Semaphore(1, true);
		// creates an array blocking queue with size capacity 3
		// Great for this situation, as there can only be 3 elves in the queue
		// Array Blocking Queues prevent any elements passed the capcity (3)
		// from being added to the Queue
		scenario.semaphoreQueue = new ArrayBlockingQueue<Elf>(3);
		// now, start the passing of time
		for(int day = 1; day < 500; day++) {
			if (day == 370){
				Santa santa = scenario.santa;

				for(Elf elf: scenario.elves) {
					elf.timetodie();
				}
				santa.timetodie();
				
			}
			// wait a day
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			// turn on December
			if (day > (365 - 31)) {
				scenario.isDecember = true;
			}
			// if the inTrouble size is 3, get the santa Semaphore
			if (scenario.inTrouble.size() == 3){
				try {
					scenario.santaSemaphore.acquire();
					// Send the elves to the door
					for (Elf elf : scenario.inTrouble){
						elf.setState(Elf.ElfState.AT_SANTAS_DOOR);

					}
					// wake up santa
					scenario.santa.WokenUpByElves();
					// release the semaphore when 
					scenario.santaSemaphore.release();
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			
			// print out the state:
			System.out.println(scenario.getNumberOfElves());
			System.out.println("***********  Day " + day + " *************************");
			scenario.santa.report();
			for(Elf elf: scenario.elves) {
				elf.report();
			}

		}
	}
	
	
}
