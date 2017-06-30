import smtplib
import sys

fromaddr = 'joaoeizidio@gmail.com' #colocar email
toaddrs  = 'imd.so.2017@gmail.com' #colocar email

msg = str(sys.argv) 

username = 'joaoeizidio@gmail.com'#colocar email de login
password = 'vasco2012' #colocar senha de login

server = smtplib.SMTP('smtp.gmail.com:587')
server.ehlo()
server.starttls()
server.login(username,password)
server.sendmail(fromaddr, toaddrs, msg)
server.quit()
print("Done")

#compilar: python3 correioeletronico.py %string
#https://myaccount.google.com/lesssecureapps  -> acessar esse link e permitir aplicativos não seguros
