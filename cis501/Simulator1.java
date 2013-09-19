import java.lang.Exception;
import java.lang.Long;
import java.lang.String;
import java.lang.System;
import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintStream;
import java.io.Reader;
import java.util.Queue;
import java.util.LinkedList;
import java.util.Iterator;
import java.util.PriorityQueue;

class Simulator
{
    
    public static void simulate(InputStream incomingStream, PrintStream outputStream) throws Exception 
    {
        // See the documentation to understand what these variables mean.
        long microOpCount;
        long instructionAddress;
        long sourceRegister1;
        long sourceRegister2;
        long destinationRegister;
        String conditionRegister;
        String TNnotBranch;
        String loadStore;
        long immediate;
        long addressForMemoryOp;
        long fallthroughPC;
        long targetAddressTakenBranch;
        String macroOperation;
        String microOperation;

        long totalMicroops = 0;
        long totalMacroops = 0;
        
        boolean exit = false;
		
		//////////////////////////////
        class Entry{
        	public long SR1, SR2, SR3;
        	public long DR1, DR2;
        	public boolean ready1, ready2, ready3;
        	public boolean isLD, isST;
        	public long address;
        	public long age;
        	public boolean issue;
        	public long FetchC, IssueC, DoneC, CommitC;
        	public String MacroOp, MicroOp;
        	public boolean isReady;
        	Entry(){
        		this.issue = false;
        		this.FetchC = 0;
        		this.IssueC = 0;
        		this.DoneC = 0;
        		this.CommitC = 0;
        	}
            
        }
        
        long j = 0;
        long[] scoreboard = new long[128];
        long CurrentC = 0;
        long phy_source1 = -1;
        long phy_source2 = -1;
        long phy_source3 = -1;
        long phy_destination1 = -1;
        long phy_destination2 = -1;
        long phy_tofree1 = -1;
        long phy_tofree2 = -1;
        long new_reg = -1;
	    long[] map = new long[50];
	    for(int i1 =0; i1 < 50; i1++)map[i1]=i1;
	    Queue<Long> freelist = new LinkedList<Long>();
        for(long i2 = 50;i2<128;i2++){
               freelist.offer(new Long(i2));
         }
        PriorityQueue<Entry> rob = new PriorityQueue<Entry>();

		//////////////////////////////
        
        BufferedReader r = new BufferedReader(new InputStreamReader(incomingStream));
        String line;
        
        outputStream.format("Processing trace...\n");
        
        while (true) {
        	if(exit)break;
        	for(int j2 = 0; j2 < 4;j2++){
            line = r.readLine();
            if (line == null) {
            	exit = true;
                break;
            }
            
            String [] tokens = line.split("\\s+");
            
            microOpCount = Long.parseLong(tokens[0]);
            instructionAddress = Long.parseLong(tokens[1], 16);
            sourceRegister1 = Long.parseLong(tokens[2]);
            sourceRegister2 = Long.parseLong(tokens[3]);
            destinationRegister = Long.parseLong(tokens[4]);
            conditionRegister = tokens[5];
            TNnotBranch = tokens[6];
            loadStore = tokens[7];
            immediate = Long.parseLong(tokens[8]);
            addressForMemoryOp = Long.parseLong(tokens[9], 16);
            fallthroughPC = Long.parseLong(tokens[10], 16);
            targetAddressTakenBranch = Long.parseLong(tokens[11], 16);
            macroOperation = tokens[12];
            microOperation = tokens[13];
            
            // For each micro-op
            totalMicroops++;

            // For each macro-op
            if (microOpCount == 1) {
                totalMacroops++;
            }
            Iterator itr = rob.iterator();
            ////////////////////////////////////commit
            for(int i3 = 0; i3 < 4; i3++){
            	if(rob.size() == 0)break;
            	if(rob.peek().DoneC <= CurrentC)
            		rob.poll();
            }
            ///////////////////////////////////issue
            int count = 0;
            while((Entry)itr.next() != null){
            	long latency = 0;
            	Entry microop = (Entry)itr.next();
            	if(microop.SR1 == -1)microop.ready1 = true;
            	else if(scoreboard[(int)microop.SR1] == 0)microop.ready1 = true;
               	if(microop.SR2 == -1)microop.ready2 = true;
            	else if(scoreboard[(int)microop.SR2] == 0)microop.ready2 = true;
               	if(microop.SR3 == -1)microop.ready3 = true;
            	else if(scoreboard[(int)microop.SR3] == 0)microop.ready3 = true;
            	if(microop.ready1 && microop.ready2 && microop.ready3)microop.isReady = true;
            	else microop.isReady = false;
            	if(microop.issue == false && microop.isReady){
            		if(loadStore.equals("L"))latency = 2;
            		else latency = 1;
            		microop.issue = true;
            		microop.IssueC = CurrentC;
            		microop.DoneC = CurrentC + latency;
            		
            		scoreboard[(int)microop.DR1] = latency;
            		count++;
            		if(count == 4)break;
            	}
            }
            //////////////////////////////////fetch and rename
            Entry nextop = new Entry();
            if(sourceRegister1 != -1){
            	phy_source1 = map[(int)sourceRegister1];
            	nextop.SR1 = phy_source1;
            }
            else nextop.SR1 =-1;
            if(sourceRegister2 != -1){
            	phy_source2 = map[(int)sourceRegister2];
            	nextop.SR2 = phy_source2;
            }
            else nextop.SR2 = -1;
            if(conditionRegister.equals("R")){
            	phy_source3 = map[49];
            	nextop.SR3 = phy_source3;
            }
            else nextop.SR3 = -1;
            if(destinationRegister !=-1){
            	phy_tofree1 = map[(int)destinationRegister];
            	new_reg = freelist.poll().longValue();
            	map[(int)destinationRegister] = new_reg;
            	phy_destination1 = new_reg;
            	freelist.offer(new Long(phy_tofree1));
            	scoreboard[(int)phy_destination1] = -1;
            	nextop.DR1 = phy_destination1;
            }
            else nextop.DR1 = -1;
            if(conditionRegister.equals("W")){
            	phy_tofree2 = map[49];
            	new_reg = freelist.poll().longValue();
            	map[49] = new_reg;
            	phy_destination2 = new_reg;
            	freelist.offer(new Long(phy_tofree2));
            	scoreboard[(int)phy_destination2] = -1;
            	nextop.DR2 = phy_destination2;
            }
            else nextop.DR2 = -1;
            rob.offer(nextop);
            if(rob.size()==8)break;
            /////////////////////////////////////advance to next cycle
            //j++;
            //if(rob.size() == 8){
            //	CurrentC ++;
            //	for(int k1 = 0; k1 < 128; k1++){
            //		if(scoreboard[k1] > 0)scoreboard[k1]--;
            //	}
            //}
            //if(j == 4){
            //	j = 0;
            //	CurrentC ++;
            //	for(int k2 = 0; k2 < 128; k2++){
            //		if(scoreboard[k2] > 0)scoreboard[k2]--;
            //	}
            //}
            //outputStream.format("%d|| ", totalMicroops);
            //outputStream.format("%d %d|| ", sourceRegister1, phy_source1);
            //outputStream.format("%d %d|| ", sourceRegister2, phy_source2);
            //outputStream.format("%d %d|| ", 49, phy_source3);
            //outputStream.format("%d %d %d|| ", destinationRegister, phy_destination1, phy_tofree1);
            //outputStream.format("%d %d %d\n", 49, phy_destination2, phy_tofree2);

            ////////////////////////////////////
        }
        	CurrentC++;
        }
        outputStream.format("Processed %d trace records.\n", totalMicroops);
        
        outputStream.format("Micro-ops: %d\n", totalMicroops);
        outputStream.format("Macro-ops: %d\n", totalMacroops);
        outputStream.format("cycles: %d\n", CurrentC);
    }
    
    public static void main(String[] args) throws Exception
    {
        InputStream inputStream = System.in;
        PrintStream outputStream = System.out;
        
        if (args.length >= 1) {
            inputStream = new FileInputStream(args[0]);
        }
        
        if (args.length >= 2) {
            outputStream = new PrintStream(args[1]);
        }
        
        Simulator.simulate(inputStream, outputStream);
    }
    
}
