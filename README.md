# Human-Machine

A demo for get linux resource information and transfer the data to another linux server.<br>

## Client

### Get linux resource infomation

- The infomation of the linux is gotten by linux command.<br>
- The infomation includes **`cpu`**, **`disk`**, **`memory`** and **`network`**.<br>
- The code is stored in **`linuxCommand.h`**.<br>

### Store the infomation in a json file

- The above information can be stored in a json file.<br>
- In this project, we use the C++ library **`jsoncpp`**. You can download it from **[jsoncpp](https://github.com/open-source-parsers/jsoncpp)**. In my project, I download the source code and use **`cmake`** to compile it. You can see the file folder **`json_include`** and file **`libjsoncpp.a`**. All the file is for the use of json operation.<br>
- The code is stored in **`linuxCommand.h`**.<br>

### Store the json file into the mysql database

- Store the above information in database.<br>
- In this project, we use the **`mysql`** database. You can download it from **[mysql](https://www.mysql.com/downloads/)**. In my project, I also download the mysql source code and use **`cmake`** to compile it. You can see the file folder **`mysql_include`** and file **`libmysqlclient.a`**. All the file is for the connection and operation of mysql.<br>
- The code is stored in **`jsonToMysql.h`**.<br>

### Use http post to transfer the json data to the server

- Transfer the linux information to another server.<br>
- In this project, we use **`http`** protocal to transfer the json data.<br>
- As the data is too long, we use the **`http post`**.(the length of the data is **`unlimited`**)<br>
- The code is stored in **`HttpRequest.h`**.<br>

## Server

### Start a servlet and handler to get the information

- Build a servlet for http post catch.<br>
- In this project , we use **`jetty`** + **`servlet`** to build a server for the http post. You can download it from [jetty](https://www.eclipse.org/jetty/download.html). In my project, I download it from [maven](https://mvnrepository.com/search?q=jetty). <br>
- After the build of above, I set a handler to catch the http post request. In my handler, it includes **`doPost`** and **`doGet`** method.<br>
- The code is stored in **`SearchServlet.java`**.<br>

### Get the json data from the http post

- Build **`getBodyData`** method to get the json data.<br>
- In this project, we use **`gson`** to transfer the data from String to jsonArray. This is a google tool, you can download it from [gson](https://mvnrepository.com/search?q=gson).
- The code is stored in **`SearchServlet.java`**.<br>

### Store the data into mysql database

- Store the json data into the mysql database.<br>
- In this project, we use **`sql`** to connect the mysql database by java language.<br>
- The code is stored in **`JsonDataToMysql.java`**.<br>
