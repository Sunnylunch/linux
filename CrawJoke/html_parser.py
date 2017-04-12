from bs4 import BeautifulSoup
import re

class Parser():
	def parser(self,cont):
		soup=BeautifulSoup(cont,"html.parser",from_encoding="utf-8")
		data=[]
		for con  in soup.find_all("span"):
			if con.string is not None and \
					bool(re.search(r"\d",con.string))==False and \
					con.string.find('.')== -1:
				data.append(con.string)
		return data
			

