import zipfile

TargetZipFileName = "temp/LispCpp.zip"
with zipfile.ZipFile(TargetZipFileName, mode='w', compression=zipfile.ZIP_STORED) as pack:
    # Run-scripts for easier running of the binaries.
    pack.writestr("Run64.bat", r"@bin\WinVs2013Release64\lcppCLICont.exe")
    pack.writestr("Run32.bat", r"@bin\WinVs2013Release32\lcppCLICont.exe")

    # Documentation
    pack.write("documents/dependencies/ezEngine.txt")
    pack.write("documents/dependencies/overview.txt")
    pack.write("documents/documentation/documentation.pdf", "documents/documentation.pdf")
    pack.write("documents/ReleasePackageReadme.md", "ReadMe.md")

    # 32 bit binaries
    pack.write("output/bin/WinVs2013Release32/ezFoundation.dll", "bin/WinVs2013Release32/ezFoundation.dll")
    pack.write("output/bin/WinVs2013Release32/ezThirdParty.dll", "bin/WinVs2013Release32/ezThirdParty.dll")
    pack.write("output/bin/WinVs2013Release32/lcppCLICont.exe", "bin/WinVs2013Release32/lcppCLICont.exe")
    pack.write("output/bin/WinVs2013Release32/lcppCoreCont.dll", "bin/WinVs2013Release32/lcppCoreCont.dll")
    pack.write("output/bin/WinVs2013Release32/lcppFoundation.dll", "bin/WinVs2013Release32/lcppFoundation.dll")

    # 64 bit binaries
    pack.write("output/bin/WinVs2013Release64/ezFoundation.dll", "bin/WinVs2013Release64/ezFoundation.dll")
    pack.write("output/bin/WinVs2013Release64/ezThirdParty.dll", "bin/WinVs2013Release64/ezThirdParty.dll")
    pack.write("output/bin/WinVs2013Release64/lcppCLICont.exe", "bin/WinVs2013Release64/lcppCLICont.exe")
    pack.write("output/bin/WinVs2013Release64/lcppCoreCont.dll", "bin/WinVs2013Release64/lcppCoreCont.dll")
    pack.write("output/bin/WinVs2013Release64/lcppFoundation.dll", "bin/WinVs2013Release64/lcppFoundation.dll")

    # Data dependencies
    pack.write("data1/base/init.lisp")
    pack.write("data1/readme.txt")
    pack.write("data1/test/oopTest.lisp")
    pack.write("data1/test/prime.lisp")
    pack.write("data1/test/stdlib.lisp")
    pack.write("data1/test/testFile.txt")
    pack.write("data1/user/ackermann.lisp")
    pack.write("data1/user/make-adder.lisp")
    pack.write("data1/user/prime.lisp")
    pack.write("data1/user/standardLibrary.lisp")
    pack.write("data1/user/standardLibraryTests.lisp")
    pack.write("data1/user/tak.lisp")
