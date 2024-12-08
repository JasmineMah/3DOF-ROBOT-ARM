#ifndef WEBSERVER_STYLE_H
#define WEBSERVER_STYLE_H

// styles.css content string.
const char* style = R"(
body {
  font-family: Arial, sans-serif;
  margin: 20px;
  padding: 0;
  background-color: #f4f4f9;
  color: #333;
  line-height: 1.6;
}

h1, h2 {
  color: #444;
}

#control-container, #information-container {
  background: #ffffff;
  padding: 20px;
  border: 1px solid #ddd;
  border-radius: 8px;
  max-width: 400px;
  margin: 0 auto 20px auto;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
}

label {
  display: block;
  margin-bottom: 5px;
  font-weight: bold;
}

input[type="number"] {
  width: calc(100% - 10px);
  padding: 8px;
  margin-bottom: 15px;
  border: 1px solid #ccc;
  border-radius: 4px;
  font-size: 1rem;
}

input[type="submit"] {
  background-color: #5cb85c;
  color: white;
  border: none;
  padding: 10px 15px;
  font-size: 1rem;
  border-radius: 4px;
  cursor: pointer;
  display: block;
  width: 100%;
  text-align: center;
}

input[type="submit"]:hover {
  background-color: #4cae4c;
}

button {
  display: block;
  margin: 0 auto 20px auto;
  padding: 10px 20px;
  font-size: 1rem;
  border: none;
  border-radius: 4px;
  background-color: #0275d8;
  color: white;
  cursor: pointer;
}

button:hover {
  background-color: #025aa5;
}
)";
#endif
