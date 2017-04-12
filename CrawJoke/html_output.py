import sys
reload(sys)
sys.setdefaultencoding('utf-8')

class Output():
	def __init__(self):
		self.datas=[]
	def collect_data(self,data):
		if data is None:
			return
		self.datas.append(data)

	def output(self):
		fout=open("/home/admin/code/web/wwwroot/joke.html","w")
		fout.write("<html>")
		fout.write("<body>")
		count=1;
		for data in self.datas:
			i=0
			fout.write("<table border=\"1\" cellspacing=\"0\">")
			fout.write("<tr>")
			fout.write("<th>")
			fout.write("%d page" % count)
			fout.write("</th>")
			fout.write("</tr>")
			while i < len(data):
				fout.write("<tr>"); 
				fout.write("<td>%d</td>" % int(i+1))
				fout.write("<td>%s</td>" % data[i])	
				fout.write("</tr>");
				i=i+1
			fout.write("</table>")
			fout.write("\n\n")
			count+=1
		fout.write("</body>")
		fout.write("</html>")
		fout.close()

