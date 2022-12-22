const button = document.getElementById('post-btn');
const firstNameInput = document.getElementById('fname');
const lastNameInput = document.getElementById('lname');
const output = document.getElementById('output')

button.addEventListener('click', async _ => {

  try {     
    const response = await fetch('http://localhost:8090/cgi/to_upper.py', {
      method: 'post',
      body: JSON.stringify({
        "first_name": firstNameInput.value,
        "last_name": lastNameInput.value
      })
    });
    console.log(response)
    var body = await response.text(); 
    var text = document.createTextNode(body.first_name + ' ' + body.last_name);

    output.appendChild(text);
  } catch(err) {
    console.error(`Error: ${err}`);
  }
});