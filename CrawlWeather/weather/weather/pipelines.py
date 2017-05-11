# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: http://doc.scrapy.org/topics/item-pipeline.html

#-*-coding:utf-8-*-
import json
import codecs
import MySQLdb


class WeatherPipeline(object):
	def __init__(self):
		self.file=codecs.open("air",mode="wb",encoding='utf-8')
		self.conn=MySQLdb.Connection(host="localhost",user="root",passwd="xxxx",db="Weather",port=3306)
	def process_item(self, item, spider):
		cur=self.conn.cursor()
		i=0
		while i < len(item['city']):
			if item['city'] is None:
				i=i+1
			else:
				line="""INSERT INTO weather VALUES ('"""
				s=((item['city'])[i])[0:6]
				line+=s
				line+="','"
				line+=((item['date'])[i])		
				line+="','"
				line+=((item['weather'])[i])		
				line+="','"
				line+=((item['air'])[i])		
				line+="','"
				line+=((item['winddir'])[i])		
				line+="','"
				line+=((item['windforce'])[i])	
				line+="""')"""	
				cur.execute(line.encode('utf-8'))
				i=i+1
		return item
