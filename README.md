LVDO
========

LVDOWin (pronounced as **ludou**) is a video steganography implementation by
StarBrilliant using DCT coefficients.

This is a Windows fork of that original repository. 
Feel free to visit it's [Github repository](https://github.com/m13253/lvdo) for more information.

What can it do?
---------------

You can convert a file into video and upload it to YouTube or many other video
sites. The receiver downloads the video and is able to extract the file using
the same parameter you encode it.


Usage
-----

This is a possible combination of parameters:

```bash
# Compile it
git clone https://github.com/m13253/lvdo.git
cd lvdo/src
make

# Encode
cat secret.mp3 | ./lvdoenc -s 640x480 -q 6 --qmin 1 --qmax 4 | x264 --input-res 640x480 --fps 1 --profile high --level 5.1 --tune stillimage --crf 22 --colormatrix bt709 --me dia --merange 0 -o public.mkv -

# Decode
ffmpeg -i public.mkv -r 1 -f rawvideo - | lvdodec -s 640x480 -q 6 --qmin 1 --qmax 4 > message.txt
```


Issues
------
During the port to Windows, arguement parsing broke. For now, I just have it hard coded.

Some parameter combination causes undecodable file.

Due to rounding errors, input file and output file is not precisely the same.
You should add some mechanism to keep file integrity.
Join the [discussion](https://github.com/m13253/lvdo/issues/4) on this issue.

Note that LVDO is not cryptographic secure! Anyone can decode the data with the
same parameters. If you have secret data, encrypt it with a password or a key
before feeding it into LVDO.


License
-------

This program is licensed under GNU General Public License version 3 or later,
the author does not provide any warranties. Users should be responsible for any
potential damage caused by this software.

Head to the file [LICENSE](LICENSE) in the code repository for the license
text.


Never abuse
-----------

I hope you will not abuse this program to do something evil.


FAQ
---

1. Why is it called LVDO?

   You will find some secrets when you spell it out in Japanesu Engurishu.

2. How do I adjust those complicated parameters?

   Do not use the maximum. Do not use the minimum either. Try to decode your self
   before publishing.

3. Why the video downloaded from YouTube can not be decoded?

   Play around with those parameters please. If you could not get it work, feel
   free to contact me for help. Also, YouTube forces the use of BT.709
   colormatrix, make sure you are using the correct x264 setting.

4. What is `clipping xxx to xxx`?

   It means the decoded file is corrupted. If it happens a lot during decoding,
   make sure you are using appropriate parameters. If it happens a lot during
   encoding, please submit an [issue](https://github.com/m13253/lvdo/issues) to
   me alongside with your parameters.

5. Any other questions?

   See if there are any [issues](https://github.com/m13253/lvdo/issues) on your
   question. If not, submit one.


Future
------

1. A friendlier GUI

2. A streaming decoder in HTML5

3. Transmission Control Layer built on top of Bitstream Transport Layer

4. Error tolerance control, see issue [#4](https://github.com/m13253/lvdo/issues/4)

5. Watermark bypassing mechanism

6. Carrier picture support, that is what the encoded picture looks like, see
   issue [#3](https://github.com/m13253/lvdo/issues/3)

7. Co-operation with some parasitic video sites


Miscellaneous
-------------

See [LVDO in action](https://www.youtube.com/watch?v=b2bLHO5KC34).

