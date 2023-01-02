const button = document.getElementById('post-btn');
const firstNameInput = document.getElementById('fname');
const lastNameInput = document.getElementById('lname');
const output = document.getElementById('output')

button.addEventListener('click', async _ => {

  try {     
    const response = await fetch('http://localhost:8090/cgi/test.py', {
      method: 'post',
      body: JSON.stringify({
        "first_name": firstNameInput.value,
        "last_name": lastNameInput.value
      })
    });
    output.textContent = await response.text();
  } catch(err) {
    console.error(`Error: ${err}`);
  }
});