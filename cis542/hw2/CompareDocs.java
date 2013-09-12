import java.util.*;
import java.io.*;

public class CompareDocs
{
    /*
     * Compare two files to see how many words they have in common.
     */
    public static int compareFiles(String file0, String file1) {

	int count = 0; // track the number of words in common
	Scanner in = null;

	// to hold the distinct words for each file
	ArrayList<String> list0 = new ArrayList<String>();
	ArrayList<String> list1 = new ArrayList<String>();

	try {
	    // read first file into a ArrayList
	    in = new Scanner(new File(file0));
	    System.out.print("Reading " + file0 + "...");
	    while (in.hasNext()) {
		String word = in.next();
		// only add the word if it's not already in the ArrayList
		if (list0.contains(word) == false) {
		    list0.add(word);
		}
	    }
	    System.out.println("done.");
	    in.close();

	    // read second file into an ArrayList
	    in = new Scanner(new File(file1));
	    System.out.print("Reading " + file1 + "...");
	    while (in.hasNext()) {
		String word = in.next();
		if (list1.contains(word) == false) {
		    list1.add(word);
		}
	    }
	    System.out.println("done.");


	    // now compare the lists one word at a time
	    System.out.println("Comparing documents...");
	    for (int i = 0; i < list0.size(); i++) {
		for (int j = 0; j < list1.size(); j++) {
		    if (list0.get(i).equals(list1.get(j))) {
			count++;
		    }
		}
	    }


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
	
	System.out.println("The files have " +  compareFiles(file0, file1) + " words in common");
    }

}