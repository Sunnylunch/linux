# Scrapy settings for myCsdnBlog project
#
# For simplicity, this file contains only the most important settings by
# default. All the other settings are documented here:
#
#     http://doc.scrapy.org/topics/settings.html
#

BOT_NAME = 'myCsdnBlog'
BOT_VERSION = '1.0'

COOKIES_ENABLED=True

ITEM_PIPELINES={
	'myCsdnBlog.pipelines.MycsdnblogPipeline':300
}

SPIDER_MODULES = ['myCsdnBlog.spiders']
NEWSPIDER_MODULE = 'myCsdnBlog.spiders'
USER_AGENT = 'mozilla/5.0'

