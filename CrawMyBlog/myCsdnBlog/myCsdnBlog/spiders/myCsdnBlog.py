import scrapy
from ..items import MycsdnblogItem
from scrapy.selector import HtmlXPathSelector
from scrapy.http import Request
from scrapy.spider import BaseSpider

class myCsdnBlogSpider(BaseSpider):
	name="myblog"
	allowed_domains=["blog.csdn.net"]
	start_urls=["http://blog.csdn.net/lf_2016/article/details/70591378"]
	def parse(self,response):
		hxs=HtmlXPathSelector(response)
		
		item=MycsdnblogItem()
		article_url=str(response.url)
		article_title=hxs.select('//div[@id="article_details"]/div/h1/span/a/text()').extract()
		item['url']=article_url.encode('utf-8')
		#item['title']=[ n.encode('utf-8') for n in article_title ]
		item['title']=article_title

		yield item
		urls=hxs.select('//li[@class="prev_article"]/a/@href').extract()
		for url in urls:
			url="http://blog.csdn.net"+url				   
			yield Request(url,callback=self.parse)





		
