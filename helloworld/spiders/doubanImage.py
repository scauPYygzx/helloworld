#-*- coding:utf-8 -*-
import urllib2
import os

from scrapy.spider import BaseSpider
from scrapy.selector import HtmlXPathSelector
from helloworld.items import DoubanImageItem
from scrapy.http import Request

class DoubanImage(BaseSpider):
    name = 'doubanImage'
    allowed_domains = ['douban.com']
    start_urls = [
            'http://movie.douban.com/subject/7051362/photos?type=S',
            'http://movie.douban.com/subject/1766551/photos?type=S',
            'http://movie.douban.com/subject/1766546/photos?type=S',
            ]
    page = 0
    count = 0
    title = ''

    def parse(self, response):
        self.page += 1
        self.count  = 0
        items = []

        hxs = HtmlXPathSelector(response)
        self.title = hxs.select('//title/text()').extract()[0].strip().replace(' ', '_')
        sites = hxs.select('//ul/li/div/a/img/@src').extract()

        for site in sites:
            site = site.replace('thumb','raw')

            item = DoubanImageItem()
            item['address'] = site
            items.append(item)

            self.count += 1
            self.download(site, self.title, self.page, self.count)
            
        #return items

        nextPageSite = hxs.select("//div[@class='paginator']/span[@class='next']/a/@href").extract()[0]
        return Request(nextPageSite, callback = self.parse)

    def download(self, site, title, page, count):
        try:
            u = urllib2.urlopen(site)
            r = u.read()
            savePath = '/home/pygzx/workspace/scrapy/helloworld/images/%s_%d_%d.%s' % (title, page, count, site.split('.')[-1])
            print 'Download...', savePath
            downloadFile = open(savePath, 'wb')
            downloadFile.write(r)
            u.close()
            downloadFile.close()
        except:
            print savePath, 'can not download.'
