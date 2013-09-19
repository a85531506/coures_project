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
import java.util.ArrayDeque;

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
        
        BufferedReader r = new BufferedReader(new InputStreamReader(incomingStream));
        String line;
/////////////////////////////////////////////
        int seqNum = 0;
        boolean next_instruction = true;
        int[] flag = new int[50];
        long cycles = 0;
        boolean sr1_ready, sr2_ready, dr_ready, flag_ready;
        int result;
        int[] GHcounter = new int[65536];
        //for(int i = 0; i<65536;i++)GHcounter[i]=0;
        long GHindex;
        long BH = 65535;
        int branch_stall = 0;
        String GHpred = "T";
        String pred;
        int fetchReady = 0;
        long[][] cache = new long[64][2];
        int[][] LRU = new int[64][2];
        long index;
        long tag;
        boolean hit;
        
        long regs = 2048;
        long robsize = 16;
        long wide = 8;
        long[] scoreboard = new long[2048];
        boolean exit1 = false, exit2 = false;
        long currentcycle = 0;
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
        for(long i2 = 50;i2<regs;i2++){
               freelist.offer(new Long(i2));
         }
        class Entry{
        	public long SR1, SR2, SR3;
        	public long DR1, DR2;
        	public boolean ready1, ready2, ready3, ready4;
        	public boolean isLD, isST;
        	public long address;
        	public boolean issue;
        	public long FetchC, IssueC, DoneC, CommitC;
        	public String MacroOp, MicroOp;
        	public boolean isReady;
        	public boolean misPred;
        	public int age;
        	Entry(){
        		this.issue = false;
        		this.FetchC = 0;
        		this.IssueC = 0;
        		this.DoneC = 0;
        		this.CommitC = 0;
        		this.ready1 = false;
        		this.ready2 = false;
        		this.ready3 = false;
        		this.ready4 = true;
        		this.isReady = false;
        		this.misPred = false;
        		this.address = 0;
        		this.age = 0;
        	}
            
        }
        ArrayDeque<Entry> rob = new ArrayDeque<Entry>();
///////////////////////////////////////////////////////////        
        outputStream.format("Processing trace...\n");
        
        while (true) {
        	if(exit2)break;///////////////////////////////////////////
        	for(int width = 0; width<wide; width++){
        		Entry temp = rob.peek();
        		if(temp != null &&temp.issue &&temp.DoneC <= currentcycle){/////////commit
        			//outputStream.format("%d|| ", temp.FetchC);
        			//outputStream.format("%d|| ", temp.IssueC);
        			//outputStream.format("%d|| ", temp.DoneC);
        			//outputStream.format("%d|| \n", currentcycle);
        			rob.poll();
        		}
        	}
        			
        	///////////////////////////////////////////////////////////issue
        	int	count = 0;
        		Iterator itr = rob.iterator();
        	while(itr.hasNext()){
        		int latency;
        		Entry nextop = (Entry)itr.next();
        		if(nextop.SR1 == -1)nextop.ready1=true;
        		else if(scoreboard[(int)nextop.SR1] == 0)nextop.ready1=true;
        		if(nextop.SR2 == -1)nextop.ready2=true;
        		else if(scoreboard[(int)nextop.SR2] == 0)nextop.ready2=true;
        		if(nextop.SR3 == -1)nextop.ready3=true;
        		else if(scoreboard[(int)nextop.SR3] == 0)nextop.ready3=true;
        		if(nextop.isLD){
        			Iterator subitr = rob.iterator();
        			while(subitr.hasNext()){
        				Entry checkop = (Entry)subitr.next();
        				if(checkop.isST&&(checkop.age<nextop.age)&&!(checkop.issue&&(checkop.DoneC<=currentcycle))){
        					nextop.ready4=false;
        			        break;
        				}
        				nextop.ready4=true;
        			}
        			if(nextop.ready1&&nextop.ready2&&nextop.ready3&&nextop.ready4)
        				nextop.isReady=true;
        		}
        		else if(nextop.ready1&&nextop.ready2&&nextop.ready3)nextop.isReady=true;
				//if(nextop.ready1&&nextop.ready2&&nextop.ready3)nextop.isReady=true;
        		if(nextop.issue == false&&nextop.isReady){
          			if(nextop.isLD){
          					nextop.address >>= 6;
        		            index=nextop.address%64;
        		            tag=nextop.address >>= 6;
                            if(cache[(int)index][0]==tag){
                            	hit=true;
                            	LRU[(int)index][0]=0;
                            	LRU[(int)index][1]=1;
                            }
                            else if(cache[(int)index][1]==tag){
                            	hit=true;
                            	LRU[(int)index][0]=1;
                            	LRU[(int)index][1]=0;
                            }
                            else{
                            	hit=false;
                            	if(LRU[(int)index][0]==0){
                            		cache[(int)index][1]=tag;
                            		LRU[(int)index][0]=1;
                            		LRU[(int)index][1]=0;
                            	}
                            	else{
                            		cache[(int)index][0]=tag;
                            		LRU[(int)index][0]=0;
                            		LRU[(int)index][1]=1;
                            	}
                            }
          				if(hit)latency=2;
          				else latency=9;
          			}
        			else latency = 1;
        			nextop.issue = true;
        			nextop.IssueC = currentcycle;
        			nextop.DoneC = currentcycle+latency;
        			if(nextop.DR1 != -1)scoreboard[(int)nextop.DR1]=latency;
        			if(nextop.DR2 != -1)scoreboard[(int)nextop.DR2]=latency;
        			count++;
        			if(nextop.misPred){
        				assert(fetchReady==-1);
        				fetchReady = 4;
        			}
        			//if(count == 4)break;
        		}
    			//outputStream.format("%d|| ", nextop.DoneC);
        		//count++;
        		if(count == wide)break;
        	}
        	////////////////////////////////////////////////////////////////////fetch and raname
        	for(int width = 0; width<wide; width++){
        		if(fetchReady > 0){
        			fetchReady--;
        			break;
        		}
        		if(fetchReady == -1)break;
        		assert(fetchReady ==0);
        		if(rob.size() == robsize)break;
        	    if(rob.size() == 0 && exit1)exit2=true;
        	//outputStream.format("%d|| ", totalMicroops+1);
        	//outputStream.format("%d|| \n", currentcycle);
            line = r.readLine();
            if (line == null) {
            	exit1 = true;
            	continue;
                //break;
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
            //outputStream.format("%d|| ", totalMicroops);
            // For each macro-op
            if (microOpCount == 1) {
                totalMacroops++;
            }
            
            if(sourceRegister1 != -1)phy_source1 = map[(int)sourceRegister1];   	           
            if(sourceRegister2 != -1)phy_source2 = map[(int)sourceRegister2];
            if(conditionRegister.equals("R"))phy_source3 = map[49];  
            if(destinationRegister !=-1){
            	phy_tofree1 = map[(int)destinationRegister];
            	new_reg = freelist.poll().longValue();
            	map[(int)destinationRegister] = new_reg;
            	phy_destination1 = new_reg;
            	freelist.offer(new Long(phy_tofree1));
            }
            if(conditionRegister.equals("W")){
            	phy_tofree2 = map[49];
            	new_reg = freelist.poll().longValue();
            	map[49] = new_reg;
            	phy_destination2 = new_reg;
            	freelist.offer(new Long(phy_tofree2));;
            }
            Entry newop = new Entry();
            seqNum++;
            if(sourceRegister1 == -1)newop.SR1 = -1;
            else newop.SR1 = phy_source1;
            if(sourceRegister2 == -1)newop.SR2 = -1;
            else newop.SR2 = phy_source2;
            if(conditionRegister.equals("R"))newop.SR3 = phy_source3;
            else newop.SR3 = -1;
            if(destinationRegister == -1)newop.DR1 = -1;
            else newop.DR1 = phy_destination1;
            if(conditionRegister.equals("W"))newop.DR2 = phy_destination2;
            else newop.DR2 = -1;
            newop.isLD = (loadStore.equals("L"))?true:false;
            newop.isST = (loadStore.equals("S"))?true:false;
            newop.address = addressForMemoryOp;
            newop.FetchC = currentcycle;
            newop.age = seqNum;
            //outputStream.format("%d|| ", newop.FetchC);
            //rob.offer(newop);
            if(destinationRegister != -1)scoreboard[(int)phy_destination1] = -1;
            if(conditionRegister.equals("W"))scoreboard[(int)phy_destination2] = -1;
            //outputStream.format("%d|| ", totalMicroops);
            //outputStream.format("%d %d|| ", sourceRegister1, phy_source1);
            //outputStream.format("%d %d|| ", sourceRegister2, phy_source2);
            //outputStream.format("%d %d|| ", 49, phy_source3);
            //outputStream.format("%d %d %d|| ", destinationRegister, phy_destination1, phy_tofree1);
            //outputStream.format("%d %d %d\n", 49, phy_destination2, phy_tofree2);
            if((TNnotBranch.equals("N")||TNnotBranch.equals("T"))&&conditionRegister.equals("R")){
            	GHindex = (instructionAddress^BH)&65535;
            	if(GHcounter[(int)GHindex]<2)GHpred = "N";
            	else GHpred = "T";
            	if(TNnotBranch.equals("N")){
            		GHcounter[(int)GHindex]--;
            		if(GHcounter[(int)GHindex]<0)GHcounter[(int)GHindex]=0;
            		BH = BH*2+1;
            	}
            	else if(TNnotBranch.equals("T")){
            		GHcounter[(int)GHindex]++;
            		if(GHcounter[(int)GHindex] >3)GHcounter[(int)GHindex]=3;
            		BH=BH*2;
            	}
            
            if(TNnotBranch.equals(GHpred))newop.misPred = false;
            else{
            	newop.misPred = true;
            	fetchReady = -1;
            }
            }
            //outputStream.format("%s %s\n", TNnotBranch, GHpred);
            rob.offer(newop);
            if(TNnotBranch.equals("T"))break;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        }
        	currentcycle++;
        	for(int phyreg = 0; phyreg<regs; phyreg++){
        		if(scoreboard[phyreg]>0)
        			scoreboard[phyreg]--;
        	}
        }
        
        outputStream.format("Processed %d trace records.\n", totalMicroops);
        
        outputStream.format("Micro-ops: %d\n", totalMicroops);
        outputStream.format("Macro-ops: %d\n", totalMacroops);
        outputStream.format("cycles: %d\n", currentcycle);
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

