import scrapy
from scrapy.spider import BaseSpider
from scrapy.selector import HtmlXPathSelector 
from scrapy.http import Request
from ..items import WeatherItem

class WeatherSpider(BaseSpider):
	name='weather'
	allowed_domains=['15tianqi.com']
	start_urls=['http://www.15tianqi.com']
	def parse(self,response):
		hxs=HtmlXPathSelector(response)
		urls=hxs.select('//ul[1]/li/a/@href').extract()
		
		special_url=[
				'http://www.15tianqi.com/shenzhen/',
				'http://www.15tianqi.com/shanxixian/',
				'http://www.15tianqi.com/hangzhou/',
				'http://www.15tianqi.com/guangzhou/',
				'http://www.15tianqi.com/beijing/',
				'http://www.15tianqi.com/shanghai/',
				'http://www.15tianqi.com/tianjin/',
				'http://www.15tianqi.com/hongkong/',
				'http://www.15tianqi.com/macao/'
				]
		for special_url in special_url:
			yield Request(special_url,callback=self.parse_item)	
	 	
		for url in urls:
			yield  Request(url,callback=self.parse_province)	

	def parse_province(self,response):
		hxs=HtmlXPathSelector(response)
		urls=hxs.select('//div[@class="mcon center"]/a/@href').extract()
	 	for url in urls:
			yield Request(url,callback=self.parse_item)			

	def parse_item(self,response):
		hxs=HtmlXPathSelector(response)
		item=WeatherItem()
		item['city']=hxs.select('//div[@id="header"]/h1/text()').extract()
		item['date']=hxs.select('//tbody/tr[1]/td[1]/span/text()').extract()
		item['weather']=hxs.select('//tbody/tr[1]/td[3]/text()').extract()
		item['air']=hxs.select('//tbody/tr[1]/td[4]/text()').extract()
		item['winddir']=hxs.select('//tbody/tr[1]/td[5]/text()').extract()
		item['windforce']=hxs.select('//tbody/tr[1]/td[6]/text()').extract()
		return item
