个人经验， go 项目的文件夹就是包，文件夹名就是包名，同一文件夹（包）下的所有文件可以视为同一文件，也就是可以随意拆分或者合并为一个或多个 go 文件都没有影响，文件名对程序也没有影响，同一文件夹（包）下的所有文件， package name 必须一致。要使用其他文件夹（包）下的资源，使用 import 导包， import 导包的路径是相对于 gopath 的相对路径

