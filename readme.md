
shellpost is a fork of demiurge. For a start I struggled with the name and then I just wanted to add a few more features like multiple uploads per message.

Works with linux and OSX 

Uses config file ~/.config/.shellpostrc or whatever is defined in SHELLPOSTRC environment variable. 

I haven't done c for a very, very long time. That's my excuse and I'm sticking to it. 

## How to use

shellpost highly uses the UNIX philosophy

``` shell
./shellpost -s "Hello this is a status" -v "unlisted" 
# Post "Hello this is a status" with the unlisted scope

./shellpost -F "filename.jpg" -s "ebin maymay :DDD" 
# Post "ebin maymay" with "filename.jpg" attatched, the file description will be
# the same as the filename.


./shellpost -v private -t "Some title text" -D "File 1 alt test/description" -F file1.png -D "File 2 alt text/description" 
-F file2.png -s "Here is the body of the text #shellpost" 
#Post 2 files with different descriptions with a topic (not marked sensitive) with the private scope.


for file in *.jpg; do ./shellpost -F $file -s "sbammin :DD"; done 
# Posts all jpgs files in different statuses

cat /etc/passwd | ./shellpost 
# Posts the output of /etc/passwd
```

shellpost is a tool to make fedi bots using shell scripts rather than a
client you\'d use. For example:

``` shell
export x=$(curl https://some-website.tld)
./shellpost -s "current weather: $x"
```

