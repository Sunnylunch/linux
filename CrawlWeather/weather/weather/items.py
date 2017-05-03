# Define here the models for your scraped items
#
# See documentation in:
# http://doc.scrapy.org/topics/items.html

from scrapy.item import Item, Field

class WeatherItem(Item):
	city=Field()
	date=Field()
	weather=Field()
	air=Field()
	winddir=Field()
	windforce=Field()
