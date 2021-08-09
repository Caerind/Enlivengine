# 1 - update
sudo apt-get update
sudo apt-get upgrade
sudo apt-get update

# 2 - ip
sudo apt-get install net-tools -y
sudo ifconfig

# 3 - ftp server
sudo apt-get install vsftpd -y
sudo vim /etc/vstfpd.conf 
#Uncomment write_enable=YES to allow the submit of executables on the server
sudo service vsftpd restart

# 4 - custom user
sudo useradd github-actions-user --create-home
sudo mkdir /home/github-actions-user/website
sudo mkdir /home/github-actions-user/website/builds
sudo chmod 700 -R /home/github-actions-user/
sudo chown -R github-actions-user /home/github-actions-user/
sudo chgrp -R github-actions-user /home/github-actions-user/

# 5 - web server
sudo apt-get install apache2 php -y
sudo rm -r /var/www/html
sudo ln -s /home/github-actions-user/website /var/www/html
sudo vim /etc/apache2/apache2.conf
#Change User and Group to github-actions-user
sudo service apache2 restart
