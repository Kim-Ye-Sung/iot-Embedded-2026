from flask import Flask, render_template

app = Flask(__name__)

@app.route('/')
def index():
   return render_template('index.html')
   
@app.route('/user/')
def user_profile():
   user = '홍길동 '
   user_age = 50
   return render_template('index.html', name=user, age=user_age)
   
if __name__ == "__main__":
   app.run(host="0.0.0.0", debug=True)
