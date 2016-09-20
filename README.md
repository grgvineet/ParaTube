# ParaTube

ParaTube is an attempt to create a cross-platform GUI YouTube downloader. With the magic of multi-threading it can download several parts of video simultaneously thus utilising maximum available bandwidth. Currently it supports downloading a video as well as a playlist. Download dialog show all available formats with their sizes, user can select it accordingly.

Currently it something looks like this :

Main Window

<img src="https://github.com/grgvineet/ParaTube/raw/master/screenshots/mainwindow.jpeg" width="50%">


Enter URL dialog

<img src="https://github.com/grgvineet/ParaTube/raw/master/screenshots/videourl.jpeg" width="50%">


Download dialog

<img src="https://github.com/grgvineet/ParaTube/raw/master/screenshots/downloaddialog.jpeg" width="50%">

### Steps to run
  - Download or clone source code
  - Import project Qt Creator
  - Hit Compile and Run 


### TODO
  - Refactor code to give a plugin mechanism so that anybody can write plugins for it any size
  - Show history in table on MainWindow
  - Add a Settings, where user can select how many threads to use, default download path etc.
  - Add download queue functionality so that it can automatically start next
  - Create packages for Linux, Windows and Mac and add it here so that anybody can download and install
