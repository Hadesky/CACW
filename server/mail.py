import sys
import httplib2

urlstr = 'http://dzyone.sinaapp.com/email.php'
httpclient = httplib2.Http('.cache')

res, content = httpclient.request(urlstr, "POST", headers={'email':sys.argv[1], 'code':sys.argv[2]})
