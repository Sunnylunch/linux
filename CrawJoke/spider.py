#!/usr/bin/python
import html_parser
import html_output
import re
import requests

class SpiderMain():
	def __init__(self):
		self.parser=html_parser.Parser()
		self.output=html_output.Output()

	def craw(self,root_url):
		page=1
		header={"User-Agent":"Mozilla/5.0"}
		while page <15:
			try:
				url=root_url+str(page)
				cont=requests.get(url,headers=header)
				data=self.parser.parser(cont.content)
				self.output.collect_data(data)
				page=page+1	
			except:
				print "craw failed"
				page=page+1
		self.output.output()

if __name__=="__main__":
	root_url="http://www.qiushibaike.com/text/page/"
	obj_spider=SpiderMain()
	obj_spider.craw(root_url)
