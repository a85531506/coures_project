package data;
import java.io.*;
import java.net.URL;
import java.util.Arrays;
import java.util.StringTokenizer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;

public class VistorData {
	/*public void getResource() throws IOException{
		URL fileURL = this.getClass().getResource("monetate_data.tsv");
	}*/
	public static void main(String[] args) {
		ArrayList<Visitor> visitor = new ArrayList<Visitor>();
		HashMap<String, Integer> countryMap = new HashMap<String, Integer>(); 
		HashMap<String, Integer> screen = new HashMap<String, Integer>();
		int[] hour = new int[24];
		int add = 0;
		float money = 0;
		try{
			VistorData res = new VistorData();
			//URL fileURL = res.getClass().getResource("monetate_data.tsv");
			//InputStream is = getClass().
			File tsv = new File("monetate_data.tsv");
		    BufferedReader br = new BufferedReader(new FileReader(tsv));
		    String line = "";
		    while((line = br.readLine())!=null){
		    	Visitor temp = new Visitor();
		    	String[] str;
		    	str = line.split("\t");
		    	temp.time = Integer.parseInt(str[0].substring(11, 13));
		    	temp.add = str[1];
		    	temp.purchase = Integer.parseInt(str[2]);
		    	temp.spend = Float.parseFloat(str[3]);
		    	temp.country = str[4];
		    	temp.screen = str[5] + "*" + str[6];
		    	visitor.add(temp);
		    }
		    br.close();
		}catch(FileNotFoundException e){
			e.printStackTrace();
		}catch(IOException e){
			e.printStackTrace();
		}		
    for(int i = 0; i < visitor.size(); i++){
    	Visitor tmp = (Visitor)visitor.get(i);
    	int count = tmp.getTime();
    	hour[count]++;
    	if(tmp.add.equals("true"))add++;
    	money = money + tmp.spend;
    	String countryTemp = tmp.country;
    	if(countryMap.containsKey(countryTemp)){
    		int countryVisitor = countryMap.get(countryTemp);
    		countryMap.put(countryTemp, countryVisitor+1);
    	}
    	else{
    		countryMap.put(countryTemp, 1);
    	}
    	String scrtmp = tmp.screen;
    	if(screen.containsKey(scrtmp)){
    		int scrNum = screen.get(scrtmp);
    		screen.put(scrtmp, scrNum+1);
    	}
    	else{
    		screen.put(scrtmp, 1);
    	}
    }
    
    int max = 0;
    int maxHr=0;
    for (int i = 0; i < 24; i++){
    	if(hour[i]>max){
    		max = hour[i];
    		maxHr = i;
    	}
    }
    Iterator<Integer> visCol = countryMap.values().iterator();
    int[] visNum = {0, 0, 0};
    while(visCol.hasNext()){
    	int j = visCol.next();
    	if(j > visNum[0]){
    		visNum[2] = visNum[1];
    		visNum[1] = visNum[0];
    		visNum[0] = j;
    	}
    	else if(j > visNum[1]){
    		visNum[2] = visNum[1];
    		visNum[1] = j;
    	}
    	else if(j > visNum[2]){
    		visNum[2] = j;
    	}
    }
    String[] popCount = new String[3];
    for (String countName : countryMap.keySet()){
    	if(countryMap.get(countName) == visNum[0])
    		popCount[0] = countName;
    	if(countryMap.get(countName) == visNum[1])
    		popCount[1] = countName;
        if(countryMap.get(countName) == visNum[2])
    		popCount[2] = countName;
    }
    Iterator<Integer> scrCol = screen.values().iterator();
    int screenNum = 0;
    while(scrCol.hasNext()){
    	int j = scrCol.next();
    	if(j > screenNum)screenNum = j;
    }
    String popScr = "";
    for (String scrres : screen.keySet()){
    	if(screen.get(scrres) == screenNum){
    		popScr = scrres;
    	}
    }
    System.out.println("Most visitor start shopping at " + maxHr + " o'clock.");
    System.out.println((double)add/visitor.size()*100 + "% visitors added goods to their cart.");
    System.out.println("The 3 top countries visitors came from is " + popCount[0] + "," + popCount[1] + "," + popCount[2]);
    System.out.println("The most popular screen resolution is " + popScr);
    System.out.println("All of the visitors spent $" + money);
	}
}
