import java.util.*;
import java.io.*;

public class CompareDocs2
{
    /*
     * Compare two files to see how many words they have in common.
     */
    public static int compareFiles(String file0, String file1) {

	int count = 0; // track the number of words in common
	BufferedReader in = null;

	// to hold the distinct words for each file
	LinkedList<String> list0 = new LinkedList<String>();
	LinkedList<String> list1 = new LinkedList<String>();

	try {
	    // read first file into a ArrayList
	    in = new BufferedReader(new FileReader(file0));
	    System.out.print("Reading " + file0 + "...");
	    String line;
	    while ((line = in.readLine()) != null) {
		//System.out.println("Read: " + line);
		// tokenize the line
		StringTokenizer tok = new StringTokenizer(line);
		while (tok.hasMoreTokens()) {
		    // only add the word if it's not already in the ArrayList
		    String word = tok.nextToken();
		    if (list0.contains(word) == false) {
			list0.add(word);
		    }
		}
	    }
	    System.out.println("done.");
	    in.close();

	    // read second file into an ArrayList
	    in = new BufferedReader(new FileReader(file1));
	    System.out.print("Reading " + file1 + "...");
	    while ((line = in.readLine()) != null) {
		StringTokenizer tok = new StringTokenizer(line);
		while (tok.hasMoreTokens()) {
		    String word = tok.nextToken();
		    if (list1.contains(word) == false) {
			list1.add(word);
		    }
		}
	    }
	    System.out.println("done.");


	    // now compare the lists one word at a time
	    System.out.println("Comparing documents...");
	    //for (int i = 0; i < list0.size(); i++) {
		for (int j = 0; j < list1.size(); j++) {
		    if (list0.contains(list1.get(j))) {
			count++;
		    }
		}
	    //}


	}
	catch (Exception e) { e.printStackTrace(); }
	finally { try { in.close(); } catch (Exception e) { } }

	return count;
    }


    public static void main(String[] args)
    {
	if (args.length < 2) {
	    System.out.println("Please specify the names of the files!");
	    System.exit(0);
	}
	
	// get the filenames
	String file0 = args[0];
	String file1 = args[1];
	long startTime = System.nanoTime();
	System.out.println("The files have " +  compareFiles(file0, file1) + " words in common");
        long endTime = System.nanoTime();
        long totalTime = endTime - startTime;
        System.out.println("The total time is " + totalTime);
    }

}
