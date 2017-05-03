# Scrapy settings for weather project
#
# For simplicity, this file contains only the most important settings by
# default. All the other settings are documented here:
#
#     http://doc.scrapy.org/topics/settings.html
#

BOT_NAME = 'weather'
BOT_VERSION = '1.0'

COOKIES_ENABLED=True

ITEM_PIPELINES={
	'weather.pipelines.WeatherPipeline':300
}
SPIDER_MODULES = ['weather.spiders']
NEWSPIDER_MODULE = 'weather.spiders'
USER_AGENT = 'mozilla/5.0'

