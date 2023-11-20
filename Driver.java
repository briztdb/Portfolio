package lab;

import java.sql.*;
import java.util.Scanner;

public class Driver {

	public static void main(String[] args) {
		Connection con = null;
		Statement stmt = null;
		try {
			//1. Register JDBC Driver
			Class.forName("com.mysql.cj.jdbc.Driver");
			
			//2. Establish Connection
			con = DriverManager.getConnection("jdbc:mysql://localhost:3306/VehicleOffice", "root", "*********");
			stmt = con.createStatement();
			
			//3. creating database
			String sql = "CREATE DATABASE IF NOT EXISTS VehicleOffice";
			stmt.executeUpdate(sql);
			
			//4. Select Database
			sql = "use VehicleOffice";
			stmt.executeUpdate(sql);
			
			//5.1. creating branch table
			sql = "CREATE TABLE IF NOT EXISTS Branch("
					+ "branch_id INT NOT NULL PRIMARY KEY,"
					+ "branch_name VARCHAR(20) NOT NULL,"
					+ "branch_addr VARCHAR(50),"
					+ "branch_city VARCHAR(20) NOT NULL,"
					+ "branch_phone INT);";
			stmt.executeUpdate(sql);
			
			//5.2. creating driver table
			sql = "CREATE TABLE IF NOT EXISTS Driver("
					+ "driver_ssn INT NOT NULL PRIMARY KEY,"
					+ "driver_name VARCHAR(20),"
					+ "driver_addr VARCHAR(50),"
					+ "driver_city VARCHAR(20),"
					+ "driver_birthdate DATE,"
					+ "driver_phone INT);";
			stmt.executeUpdate(sql);
			
			//5.3. creating license table
			sql = "CREATE TABLE IF NOT EXISTS License("
					+ "license_no INT NOT NULL PRIMARY KEY,"
					+ "driver_ssn INT,"
					+ "license_type CHAR(1),"
					+ "license_class INT,"
					+ "license_expiry DATE,"
					+ "issue_date DATE,"
					+ "branch_id INT);";
			stmt.executeUpdate(sql);
			
			//5.4. creating exam table
			sql = "CREATE TABLE IF NOT EXISTS Exam("
					+ "driver_ssn INT NOT NULL,"
					+ "branch_id INT NOT NULL,"
					+ "exam_date DATE,"
					+ "exam_type CHAR(1),"
					+ "exam_score INT,"
					+ "PRIMARY KEY(driver_ssn, branch_id, exam_date));";
			stmt.executeUpdate(sql);
			
			sql = "DELETE FROM Branch;";
			stmt.executeUpdate(sql);
			sql = "DELETE FROM Driver;";
			stmt.executeUpdate(sql);
			sql = "DELETE FROM License;";
			stmt.executeUpdate(sql);
			sql = "DELETE FROM Exam;";
			stmt.executeUpdate(sql);
			//6.1. insert tuples into branch
			sql = "INSERT INTO Branch VALUES(10,'Main Hoboken', '1234 Main St.', 'Hoboken', 5551234);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO Branch VALUES(20,'NYC 33rd street', '2320 33rd street', 'NYC', 5552331);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO Branch VALUES(30,'West Creek','251 Creek Rd.','Newark',5552511);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO Branch VALUES(40,'Blenheim','1342 W. 22 Ave','Princeton',5551342);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO Branch VALUES(50,'NYC 98 street','340 98th St.','NYC',5214202);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO Branch VALUES(60,'NYC 4th street','21 4th St.','NYC',5214809);";
			stmt.executeUpdate(sql);
			
			//6.2. insert tuples into Driver
			sql = "INSERT INTO Driver VALUES(11111111,'Bob Smith','111 E.11 St.','Hoboken','1975-01-01',5551111);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO Driver VALUES(22222222,'John Walters','222 E.22 St.','Princeton','1976-02-02',5552222);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO Driver VALUES(33333333,'Troy Rops','333 W.33 Ave','NYC','1970-03-03',5553333);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO Driver VALUES(44444444,'Kevin Mark','444 E.4 Ave','Hoboken','1974-04-04',5554444);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO Driver VALUES(55555555,'Amelie Kim','63 Main St.','Hoboken','2000-09-10',5551456);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO Driver VALUES(66666666,'Mary Gup','47 W 13th St.','NYC','1998-12-31',5552315);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO Driver VALUES(77777777,'Clark Johnson','36 E 8th St.','NYC','1999-10-01',5559047);";
			stmt.executeUpdate(sql);
			
			//6.3. insert tuples into License
			sql = "INSERT INTO License VALUES(1,11111111,'D',5,'2022-05-24','2017-05-25',20);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO License VALUES(2,22222222,'D',5,'2023-09-29','2016-08-29',40);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO License VALUES(3,33333333,'L',5,'2022-12-27','2017-06-27',20);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO License VALUES(4,44444444,'D',5,'2022-08-30','2017-08-30',40);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO License VALUES(5,77777777,'D',3,'2025-08-17','2020-08-17',50);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO License VALUES(6,66666666,'D',1,'2024-01-12','2020-01-11',50);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO License VALUES(7,44444444,'L',5,'2023-01-31','2020-12-31',30);";
			stmt.executeUpdate(sql);
			
			//6.4. insert tuples into Exam
			sql = "INSERT INTO Exam VALUES(11111111,20,'2017-05-25','D',79);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO Exam VALUES(22222222,30,'2016-05-06','L',25);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO Exam VALUES(22222222,40,'2016-06-10','L',51);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO Exam VALUES(33333333,10,'2017-07-07','L',45);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO Exam VALUES(33333333,20,'2017-07-27','L',61);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO Exam VALUES(44444444,10,'2017-07-27','L',71);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO Exam VALUES(44444444,20,'2017-08-30','L',65);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO Exam VALUES(44444444,40,'2017-09-01','L',82);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO Exam VALUES(11111111,20,'2017-12-02','L',67);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO Exam VALUES(22222222,40,'2016-08-29','D',81);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO Exam VALUES(33333333,20,'2017-06-27','L',49);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO Exam VALUES(44444444,10,'2019-04-10','D',80);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO Exam VALUES(77777777,30,'2020-12-31','L',90);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO Exam VALUES(77777777,30,'2020-10-30','L',40);";
			stmt.executeUpdate(sql);
			sql = "INSERT INTO Exam VALUES(66666666,40,'2020-02-03','D',90);";
			stmt.executeUpdate(sql);
			
			Scanner scan = new Scanner(System.in);
			System.out.println("Welcome to the Vehicle Office Database!\n"
					+ "Search through License, Exam, Driver, Branch information.\nYou can also "
					+ "search through expired licenses or errors in exam information.\n\n"
					+ "Type 'help' for command information or 'quit' to exit.");
			String input;
			
			while(true) {
				try {
					input = scan.nextLine();
					if(input.toLowerCase().equals("quit") | input.toLowerCase().equals("exit")) {
						break;
					}
					if(input.toLowerCase().equals("help")) {
						System.out.println("You can input the following options: \n"
								+ "\tlookup :\n\t\t"
								+ "-l 'driver name' : Get license information for specified driver.\n\t\t"
								+ "-e 'driver name' : Get exam information for specified driver.\n\t\t"
								+ "-b 'branch name' : Get drivers licensed through specified branch.\n\t\t"
								+ "-c 'city name' : Get information for branches located in specified city\n\t\t"
								+ "--expired : Get information for drivers with expired licenses (Atleast 3 months by Nov 2, 2023)\n"
								+ "\t\terror : Search for the following errors in the exam table\n\t\t\t"
								+ "Type I : Get driver names with unmatching branch ID between license and passing exam\n\t\t\t"
								+ "Type II : Get driver names with license issue dates before latest exam taken\n\t\t\t"
								+ "Type III : Get driver names with license type does not match their latest exam type\n\t\t\t"
								+ "Type IV : Get driver names who did not pass any exam but were issued a license\n\t\t\t"
								+ "Type V : Get driver names who did not take an exam but were issued a license");
						continue;
					}
					String[] inputArgs = input.split(" ", 3);
					if(inputArgs.length < 2) {
						System.out.println("Incorrect Argument count. Please use 'help' command to view proper syntax");
						continue;
					}
					// Get License Information for Specific Driver (lookup -l ...)
					if(inputArgs[1].equals("-l")) {
						if(inputArgs.length != 3) {
							System.out.println("Incorrect Argument count. Please use 'help' command to view proper syntax");
							continue;
						}
						String dname = inputArgs[2].substring(1, inputArgs[2].length()-1);
						sql = "SELECT license_type, license_expiry, issue_date, branch_name "
								+ "FROM License l NATURAL JOIN Driver d, Branch b "
								+ "WHERE driver_name='" + dname + "' AND b.branch_id=l.branch_id;";
						Statement s = con.createStatement();
						s.executeQuery(sql);
						ResultSet rs = s.getResultSet();
						int count = 0;
						while(rs.next()) {
							String ltype = rs.getString("license_type");
							String lexpiry = rs.getString("license_expiry");
							String lissue = rs.getString("issue_date");
							String branch_name = rs.getString("branch_name");
							System.out.println("license_type = " + ltype + ", expiry_date = " + lexpiry + ","
									+ " lissue = " + lissue + ", branch_name = " + branch_name);
							++count;
						}
						rs.close();
						s.close();
						System.out.println(count + " rows were retrieved");
						continue;
					}
					
					// Get Exam Information for Specific Driver (lookup -e ...)
					if(inputArgs[1].equals("-e")) {
						if(inputArgs.length != 3) {
							System.out.println("Incorrect Argument count. Please use 'help' command to view proper syntax");
							continue;
						}
						String dname = inputArgs[2].substring(1, inputArgs[2].length()-1);
						sql = "SELECT driver_ssn, branch_id, exam_date, exam_type, exam_score, branch_name "
								+ "FROM Branch NATURAL JOIN Exam NATURAL JOIN Driver "
								+ "WHERE driver_name='" + dname + "';";
						Statement s = con.createStatement();
						s.executeQuery(sql);
						ResultSet rs = s.getResultSet();
						int count = 0;
						while(rs.next()) {
							int driver_ssn = rs.getInt("driver_ssn");
							int branch_id = rs.getInt("branch_id");
							String exam_date = rs.getString("exam_date");
							String exam_type = rs.getString("exam_type");
							int exam_score = rs.getInt("exam_score");
							String branch_name = rs.getString("branch_name");
							System.out.println("driver_ssn = " + driver_ssn + ", branch_id = " + branch_id + ","
									+ " exam_date = " + exam_date + ", exam_type = " + exam_type + ","
											+ " exam_score = " + exam_score + ", branch_name = " + branch_name);
							++count;
						}
						rs.close();
						s.close();
						System.out.println(count + " rows were retrieved");
						continue;
					}
					
					// Get drivers licensed through specified branch (lookup -b ...)
					if(inputArgs[1].equals("-b")) {
						if(inputArgs.length != 3) {
							System.out.println("Incorrect Argument count. Please use 'help' command to view proper syntax");
							continue;
						}
						String bname = inputArgs[2].substring(1, inputArgs[2].length()-1);
						sql = "SELECT d.driver_name, d.driver_addr, d.driver_city, d.driver_phone, l.license_type "
								+ "FROM Driver d, Branch b, License l "
								+ "WHERE b.branch_name='" + bname + "' AND "
										+ "b.branch_id=l.branch_id AND "
										+ "d.driver_ssn=l.driver_ssn;";
						Statement s = con.createStatement();
						s.executeQuery(sql);
						ResultSet rs = s.getResultSet();
						int count = 0;
						while(rs.next()) {
							String driver_name = rs.getString(1);
							String driver_addr = rs.getString(2);
							String driver_city = rs.getString(3);
							String driver_phone = rs.getString(4);
							String license_type = rs.getString(5);
							System.out.println("driver_name = " + driver_name + ", driver_addr = " + driver_addr + ","
									+ " driver_city = " + driver_city + ", driver_phone = " + driver_phone + ","
											+ " license_type = " + license_type);
							++count;
						}
						rs.close();
						s.close();
						System.out.println(count + " rows were retrieved");
						continue;
					}
					
					// Get information for branches located in specified city (lookup -c ...)
					if(inputArgs[1].equals("-c")) {
						if(inputArgs.length != 3) {
							System.out.println("Incorrect Argument count. Please use 'help' command to view proper syntax");
							continue;
						}
						String cname = inputArgs[2].substring(1, inputArgs[2].length()-1);
						sql = "SELECT branch_name, branch_addr, branch_phone, COUNT(*) "
								+ "FROM Branch b, License l "
								+ "WHERE branch_city='" + cname + "' "
										+ "GROUP BY b.branch_id;";
						Statement s = con.createStatement();
						s.executeQuery(sql);
						ResultSet rs = s.getResultSet();
						int count = 0;
						while(rs.next()) {
							String branch_name = rs.getString(1);
							String branch_addr = rs.getString(2);
							String branch_phone = rs.getString(3);
							int numLicenses = rs.getInt(4);
							System.out.println("branch_name = " + branch_name + ", branch_addr = " + branch_addr + ","
									+ " branch_phone = " + branch_phone + ", Licenses issued = " + numLicenses);
							++count;
						}
						rs.close();
						s.close();
						System.out.println(count + " rows were retrieved");
						continue;
					}
					
					// Find drivers and their phone numbers whose licenses are more than 3 months expired (lookup --expired)
					if(inputArgs[1].equals("--expired")) {
						if(inputArgs.length != 2) {
							System.out.println("Incorrect Argument count. Please use 'help' command to view proper syntax");
							continue;
						}
						sql = "SELECT driver_name, driver_phone "
								+ "FROM Driver d, License l "
								+ "WHERE d.driver_ssn=l.driver_ssn AND l.license_expiry<'2023-08-02';";
						Statement s = con.createStatement();
						s.executeQuery(sql);
						ResultSet rs = s.getResultSet();
						int count = 0;
						while(rs.next()) {
							String driver_name = rs.getString(1);
							String driver_phone = rs.getString(2);
							System.out.println("driver_name = " + driver_name + ", driver_phone = " + driver_phone);
							++count;
						}
						rs.close();
						s.close();
						System.out.println(count + " rows were retrieved");
						continue;
					}
					
					// Find errors in the exam table (lookup error)
					if(inputArgs[1].equals("error")) {
						if(inputArgs.length != 2) {
							System.out.println("Incorrect Argument count. Please use 'help' command to view proper syntax");
							continue;
						}
						
						// Type 1 errors
						sql = "SELECT driver_name "
								+ "FROM Driver d NATURAL JOIN Exam e, License l "
								+ "WHERE l.driver_ssn=d.driver_ssn AND e.branch_id<>l.branch_id "
								+ "EXCEPT "
								+ "SELECT driver_name "
								+ "FROM Driver d NATURAL JOIN Exam e, License l "
								+ "WHERE l.driver_ssn=d.driver_ssn AND e.branch_id=l.branch_id;";
						Statement s = con.createStatement();
						s.executeQuery(sql);
						ResultSet rs = s.getResultSet();
						int count = 0;
						System.out.println("Type 1 errors: ");
						while(rs.next()) {
							String driver_name = rs.getString(1);
							System.out.println("driver_name = " + driver_name);
							++count;
						}
						System.out.println(count + " rows were retrieved\n\n");
						
						//Type 2 Errors
						sql = "SELECT driver_name "
								+ "FROM Driver d NATURAL JOIN Exam e, License l "
								+ "WHERE l.driver_ssn=d.driver_ssn AND e.exam_date >= ALL (SELECT exam_date "
								+ "FROM Driver d2 NATURAL JOIN Exam e2 "
								+ "WHERE d2.driver_ssn=d.driver_ssn) AND l.issue_date<e.exam_date;";
						s.executeQuery(sql);
						rs = s.getResultSet();
						count = 0;
						System.out.println("Type 2 errors: ");
						while(rs.next()) {
							String driver_name = rs.getString(1);
							System.out.println("driver_name = " + driver_name);
							++count;
						}
						System.out.println(count + " rows were retrieved\n\n");
						
						// Type 3 errors
						sql = "SELECT driver_name "
								+ "FROM Driver d NATURAL JOIN Exam e, License l "
								+ "WHERE l.driver_ssn=d.driver_ssn AND e.exam_date >= ALL (SELECT exam_date "
								+ "FROM Driver d2 NATURAL JOIN Exam e2 "
								+ "WHERE d2.driver_ssn=d.driver_ssn) AND l.license_type<>e.exam_type;";
						s.executeQuery(sql);
						rs = s.getResultSet();
						count = 0;
						System.out.println("Type 3 errors: ");
						while(rs.next()) {
							String driver_name = rs.getString(1);
							System.out.println("driver_name = " + driver_name);
							++count;
						}
						System.out.println(count + " rows were retrieved\n\n");
						
						//Type 4 errors
						sql = "SELECT DISTINCT driver_name "
								+ "FROM Driver d NATURAL JOIN Exam e, License l "
								+ "WHERE 70 > ALL (SELECT exam_score "
								+ "FROM Driver d2 NATURAL JOIN Exam e2 "
								+ "WHERE d2.driver_ssn=d.driver_ssn) AND l.driver_ssn=d.driver_ssn;";
						s.executeQuery(sql);
						rs = s.getResultSet();
						count = 0;
						System.out.println("Type 4 errors: ");
						while(rs.next()) {
							String driver_name = rs.getString(1);
							System.out.println("driver_name = " + driver_name);
							++count;
						}
						System.out.println(count + " rows were retrieved\n\n");
						
						//Type 5 errors
						sql = "SELECT DISTINCT driver_name "
								+ "FROM Driver d NATURAL JOIN Exam e "
								+ "WHERE d.driver_ssn NOT IN (SELECT driver_ssn "
								+ "FROM Exam e);";
						s.executeQuery(sql);
						rs = s.getResultSet();
						count = 0;
						System.out.println("Type 5 errors: ");
						while(rs.next()) {
							String driver_name = rs.getString(1);
							System.out.println("driver_name = " + driver_name);
							++count;
						}
						System.out.println(count + " rows were retrieved\n\n");
						continue;
					}
					System.out.println("Command not recognized. Please use the 'help' command to view valid commands");
					continue;
				} catch (SQLException ex) {
					ex.printStackTrace();
				}
			}
			
			scan.close();
			sql = "DELETE FROM Branch;";
			stmt.executeUpdate(sql);
			sql = "DELETE FROM Driver;";
			stmt.executeUpdate(sql);
			sql = "DELETE FROM License;";
			stmt.executeUpdate(sql);
			sql = "DELETE FROM Exam;";
			stmt.executeUpdate(sql);
		}
		catch (Exception ex) {
			ex.printStackTrace();
		}
		finally {
			try {
				if(stmt!=null)
					stmt.close();
			} catch(SQLException se2) {}
			try {
				if(con!=null)
					con.close();
			} catch(SQLException se) {
				se.printStackTrace();
			}
		}
			
		
	}

}
