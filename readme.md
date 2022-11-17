
Toot is a fork of demiurge. For a start I struggled with the name and then I just wanted to add a few more features like multiple uploads per message.

Works with linux and OSX - Tested with Pleroma 2.4.4 and Mastodon 4.0.2

Uses config file ~/.config/.tootrc or whatever is defined in TOOTRC environment variable. 

```TOOTRC=~/.config/.myotherserver.rc toot -s "This is using a different config file"```

I haven't done c for a very, very long time. That's my excuse and I'm sticking to it. 

## How to use

toot uses the UNIX philosophy

``` shell
./toot -s "Hello this is a status" -v "unlisted" 
# Post "Hello this is a status" with the unlisted scope

./toot -F "filename.jpg" -s "ebin maymay :DDD" 
# Post "ebin maymay" with "filename.jpg" attatched, the file description will be
# the same as the filename.


./toot -v private -t "Some title text" -D "File 1 alt test/description" -F file1.png -D "File 2 alt text/description" -F file2.png -s "Here is the body of the text #toot" 
#Post 2 files with different descriptions with a topic (not marked sensitive) with the private scope.


for file in *.jpg; do ./toot -F $file -s "sbammin :DD"; done 
# Posts all jpgs files in different statuses

cat /etc/passwd | ./toot 
# Posts the output of /etc/passwd
```

toot is a tool to make fedi bots using shell scripts rather than a
client you\'d use. For example:

``` shell
export x=$(curl https://some-website.tld)
./toot -s "current weather: $x"
```

