# 原生http

### Urlencoded Form

即content-type 为 x-www-form-urlencode的数据。



```go
r.Form["param"]
r.PostForm["param"]
r.FormValue("param")
r.PostFormValue("param")
```

其中r.Form和r.PostForm必须在调用ParseForm之后，才会有数据，否则则是空数组。
而r.FormValue和r.PostFormValue("lang")无需ParseForm的调用就能读取数据。

### MutilpartFrom

即content-type为form-data的数据使用ParseMutilpartFrom方法解析参数，但是不会解析url query的参数的



```go
r.Form["param"]
r.PostForm["param"]
r.FormValue("param")
r.PostFormValue("param")
r.MultipartForm.Value["param"]
```

r.MultipartForm.File包含图片数据
r.FormFile可以直接读取上传文件数据

# gin

### Parameters in path



```go
func main() {
    router := gin.Default()

    // This handler will match /user/john but will not match neither /user/ or /user
    router.GET("/user/:name", func(c *gin.Context) {
        name := c.Param("name")
        c.String(http.StatusOK, "Hello %s", name)
    })

    // However, this one will match /user/john/ and also /user/john/send
    // If no other routers match /user/john, it will redirect to /user/john/
    router.GET("/user/:name/*action", func(c *gin.Context) {
        name := c.Param("name")
        action := c.Param("action")
        message := name + " is " + action
        c.String(http.StatusOK, message)
    })

    router.Run(":8080")
}
```

### Querystring parameters



```go
func main() {
    router := gin.Default()

    // Query string parameters are parsed using the existing underlying request object.
    // The request responds to a url matching:  /welcome?firstname=Jane&lastname=Doe
    router.GET("/welcome", func(c *gin.Context) {
        firstname := c.DefaultQuery("firstname", "Guest")
        lastname := c.Query("lastname") // shortcut for c.Request.URL.Query().Get("lastname")

        c.String(http.StatusOK, "Hello %s %s", firstname, lastname)
    })
    router.Run(":8080")
}
```

### Multipart/Urlencoded Form



```go
func main() {
    router := gin.Default()

    router.POST("/form_post", func(c *gin.Context) {
        message := c.PostForm("message")
        nick := c.DefaultPostForm("nick", "anonymous")

        c.JSON(200, gin.H{
            "status":  "posted",
            "message": message,
            "nick":    nick,
        })
    })
    router.Run(":8080")
}
```

### query + post form



```go
func main() {
    router := gin.Default()

    router.POST("/post", func(c *gin.Context) {

        id := c.Query("id")
        page := c.DefaultQuery("page", "0")
        name := c.PostForm("name")
        message := c.PostForm("message")

        fmt.Printf("id: %s; page: %s; name: %s; message: %s", id, page, name, message)
    })
    router.Run(":8080")
}
```