#!/usr/bin/python
import MySQLdb
conn=MySQLdb.Connection(host="localhost",user="root",passwd="1996",db="Weather",port=3306)
cur=conn.cursor()
cur.execute('truncate weather')
cur.close()
conn.close()
