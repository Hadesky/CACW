#import sys
#import httplib2

#urlstr = 'http://dzyone.sinaapp.com/email.php'
#httpclient = httplib2.Http('.cache')

#res, content = httpclient.request(urlstr, "POST", headers={'email':sys.argv[1], 'code':sys.argv[2]})

#!/usr//bin/python
#coding=<utf-8>
import sys
import smtplib
from email.mime.text import MIMEText
from email.header import Header

sender = '453589103@qq.com'
receiver = sys.argv[1]
subject = 'CACW authcode'
smtpserver = 'smtp.qq.com'
username = '453589103@qq.com'
password = 'testmail'
content = 'welcome to register CACW account!\n\r\tyour authcode:' + sys.argv[2]

msg = MIMEText(content, "text", "utf-8")
msg['Subject'] = Header(subject, 'utf-8')

smtp = smtplib.SMTP()
smtp.connect('smtp.qq.com')
smtp.login(username, password)
smtp.sendmail(sender, receiver, msg.as_string())
smtp.quit()
