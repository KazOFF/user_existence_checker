const express = require('express')
const app = express()
const host = 'localhost'
const port = 8080

app.get('/', (req, res) => {
    res.sendFile('html/index.html', {root: __dirname });
})

app.get('/exist', (req, res) => {
    res.sendFile('html/exist.html', {root: __dirname });
})

app.get('/notexist', (req, res) => {
    res.sendFile('html/notexist.html', {root: __dirname });
})

app.get('/check', (req, res) => {
    let existence = require('bindings')('user_existence.node')
    
    try {
        if(existence.checkUserExistence(req.query['username'])){
            res.redirect(302, '/exist');
        }else{
            res.redirect(302, '/notexist');
        }
      } catch (error) {
        console.error(error);
      }
})


app.listen(port, () => {
    console.log(`http://${host}:${port}`);
})
