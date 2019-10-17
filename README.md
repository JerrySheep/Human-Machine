# Human-Machine

A demo for get linux resource information and transfer the data to another linux server.<br>

## Client

### 1. Get linux resource infomation

The infomation of the linux is gotten by linux command.<br>
The infomation includes **`cpu`**, **`disk`**, **`memory`** and **`network`**.<br>
The code is stored in **`linuxCommand.h`**.<br>

### 2. Store the infomation in a json file

The above information can be stored in a json file.<br>
In this project, we use the C++ library **`jsoncpp`**. You can download it from **[jsoncpp](https://github.com/open-source-parsers/jsoncpp)**. In my project, I download the source code and use **`cmake`** to compile it. You can see the file folder **`json_include`** and file **`libjsoncpp.a`**. All the file is for the use of json operation.
The code is stored in **`linuxCommand.h`**.<br>

### 3. Store the json file into the mysql database

Store the above information in database.<br>
In this project, we use the **`mysql`** database. You can download it from **[mysql](https://www.mysql.com/downloads/)**. In my project, I also download the mysql source code and use **`cmake`** to compile it. You can see the file folder **`mysql_include`** and file **`libmysqlclient.a`**. All the file is for the connection and operation of mysql.<br>
The code is stored in **`jsonToMysql.h`**.<br>

### 4. use http post to transfer the json data to the server

Transfer the linux information to another server.<br>
In this project, we use **`http`** protocal to transfer the json data.<br>
As the data is too long, we use the **`http post`**.(the length of the data is **`unlimited`**)<br>
The code is stored in **`HttpRequest.h`**.<br>

