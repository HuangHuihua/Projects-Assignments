import { BACKEND_PORT } from './config.js';
// A helper you may want to use when uploading new images to the server.
import { fileToDataUrl } from './helpers.js';


console.log('Let\'s go!');


const popupAlert = (errormessage)=>{

	const child = document.createElement("p");
	const childtext = document.createTextNode(errormessage);
	child.appendChild(childtext);
	document.getElementById("popup-alert-p").appendChild(child);	

	document.getElementById("popup-alert").classList.remove('hide');
}

const closepopupAlert = (event)=>{

	const popalert = document.getElementById("popup-alert");

	popalert.classList.add('hide');

	const popalert_p = document.getElementById("popup-alert-p");
	while(popalert_p.firstChild){
		popalert_p.removeChild(popalert_p.lastChild);
	}


}

document.getElementById('close-alert').addEventListener('click', closepopupAlert);



const apiCall = (path,method, payload, success) =>{

	const options = {
		method: method,
		headers:{
			'Content-type': 'application/json',
		},

	};

	if(method === 'GET'){

	}else{
		options.body = JSON.stringify(payload);
	}	

	if(localStorage.getItem('token')){
		options.headers.Authorization = `Bearer ${localStorage.getItem('token')}`;
	}

	fetch('http://localhost:5005/' + path, options)
	.then(response => {
		if(response.status === 404){
        	popupAlert('It\'s 404');
		}



		response.json()
			.then((data)=>{
				if(data.error){
					popupAlert(data.error);
				}else{
					// console.log(data);
					if(success)
						success(data);
				}
			});
	});
};


//infinite scroll
let content = document.getElementById('feed-items');
let index_i = 0; // 
let isLoading = false; //

//resize will cause index increase
window.addEventListener('resize', function() {
  //
  let pageHeight = document.documentElement.scrollHeight;
  let scrollPos = window.pageYOffset || document.documentElement.scrollTop || document.body.scrollTop || 0;

  //
  if (pageHeight - scrollPos - window.innerHeight < 500) {
    isLoading = true;
    index_i++; 

   const options = {
		method: 'GET',
		headers:{
			'Content-type': 'application/json',
		},

	};

	if(localStorage.getItem('token')){
		options.headers.Authorization = `Bearer ${localStorage.getItem('token')}`;
	}
   fetch('http://localhost:5005/job/feed?start='+`${index_i}`,options)
      
 //   	.then(response => {
	// 	if(response.status === 404){
 //        	popupAlert('It\'s 404');
	// 	}

	// 	response.json()
	// 		.then((data)=>{
	// 			if(data.error){
	// 				popupAlert(data.error);
	// 			}else{
	// 				console.log(JSON.stringify(data));
	// 				functionbody(data);
	// 			}
	// 			isLoading = false;
	// 		});
	// });
      .then(response=> {
        return response.json();
      })
      .then(function(data) {

        //functionbody(data);
        console.log(data);
				functionbody_i(data[0], content);

        // const feedDom = document.createElement('div')
        // feedDom.className = "p-3 mb-2 bg-body-tertiary bg-info-subtle text-emphasis-info";
        // feedDom.innerText = data[0].title;
        
        //const firstChild = data.
        // const feed_item_child = document.createElement('div');
        // feed_item_child.setAttribute("id", `${data[0]}-div-feed_item_child`);

        //functionbody_i(data[0], `${data[0]}-div-feed_item_child`);
        //data[0].
        //functionbody(data);
        //append new child
        // content.appendChild(feedDom);
        isLoading = false;
      });
    }
});




window.addEventListener('scroll', () => {



  let scrollHeight = document.documentElement.scrollHeight;
  let scrollTop = document.documentElement.scrollTop || document.body.scrollTop;
  let clientHeight = document.documentElement.clientHeight;

  if (scrollTop + clientHeight >= scrollHeight && !isLoading) {
    isLoading = true;
    index_i++; 

   const options = {
		method: 'GET',
		headers:{
			'Content-type': 'application/json',
		},

	};

	if(localStorage.getItem('token')){
		options.headers.Authorization = `Bearer ${localStorage.getItem('token')}`;
	}
   fetch('http://localhost:5005/job/feed?start='+`${index_i}`,options)
      
 //   	.then(response => {
	// 	if(response.status === 404){
 //        	popupAlert('It\'s 404');
	// 	}

	// 	response.json()
	// 		.then((data)=>{
	// 			if(data.error){
	// 				popupAlert(data.error);
	// 			}else{
	// 				console.log(JSON.stringify(data));
	// 				functionbody(data);
	// 			}
	// 			isLoading = false;
	// 		});
	// });
      .then(response=> {
        return response.json();
      })
      .then(function(data) {

        //functionbody(data);
        console.log(data[0]);

        
        //functionbody_i(data[0]);
        //const firstChild = data.

        // const feedDom = document.createElement('div');
        // feedDom.className = "p-3 mb-2 bg-body-tertiary bg-info-subtle text-emphasis-info";
        // feedDom.innerText = data[0].title;

        // const feedDom2 = document.createElement('div');
				functionbody_i(data[0], content);
        //content.appendChild(feedDom);

        //content.appenChild();
        isLoading = false;
      });
  }
});




//for pagination
let index = 0;
const populateFeed_second = () =>{

		const options = {
		method: 'GET',
		headers:{
			'Content-type': 'application/json',
		},

	};

	if(localStorage.getItem('token')){
		options.headers.Authorization = `Bearer ${localStorage.getItem('token')}`;
	}

	  fetch('http://localhost:5005/job/feed?start='+`${index}`,options)
    .then(response => response.json())
    .then(data => {

      if (data.length > 0) {

        // console.log(data);

        functionbody(data);
 
        document.getElementById('next-page').addEventListener('click',()=>{
        index+=5;
				populateFeed_second();
        });

        //if(index<=0) document.getElementById('previous-page').disabled = true;
        document.getElementById('previous-page').addEventListener('click',()=>{
	        if((index-=5) < 0) index =0;
	        else index-=5;
					populateFeed_second();

        });

         document.getElementById('back-first-page').addEventListener('click',()=>{
	        index=0;
					populateFeed_second();

        });      

      }

    })
    .catch(error => {
      console.error(error);
    });
}










let count=0;

const populateFeed = ()=>{
	console.log('populateFeed call');


	apiCall(`job/feed?start=${count}`, 'GET', {}, (data)=>{

		// console.log('data',data);
		document.getElementById('feed-items').textContent ='';
		for(const feedItem of data){

			const removejobbutton = document.createElement('button');



			const labelName = document.createElement('label');
			labelName.className = "form-label p-3 mb-2 bg-info-subtle text-emphasis-info";

			//job creator
			apiCall(`user?userId=${feedItem.creatorId}`, 'GET', {}, (datauser)=>{
				// console.log('userdata',datauser);

				if(datauser.id === feedItem.creatorId){
					labelName.innerText = "Job made by " + datauser.name; 						
				}

			});
			document.getElementById('feed-items').appendChild(labelName);

			//date and interval

			createTime(feedItem);


			// if(result < 24){
			// 	dateText.className = "col";
			// 	dateText.innerText = result + "ago";

			// }else{
			// 	dateText.innerText = date_;
			// }

			// document.getElementById('feed-items').appendChild(dateText);

			//title , id , image....


			// const jobContent = (Name_, feedItem_stuff, create_Element, class_Name)=>{
			// 	Name_ = document.createElement(create_Element);
			// 	Name_.className = class_Name;
			// 	Name_.setAttribute("id", )
			// 	Name_.innerText = ""
			// }


			const feedDom = document.createElement('div');
			feedDom.className = "p-3 mb-2 bg-body-tertiary";
			feedDom.innerText = feedItem.title;
			document.getElementById('feed-items').appendChild(feedDom);
			// console.log(feedItem);

			const feedDom2 = document.createElement('div');
			feedDom2.className = "text-primary col";
			feedDom2.innerText = "id: " + feedItem.id;
			document.getElementById('feed-items').appendChild(feedDom2);

			const feedDom3 = document.createElement('img');
			feedDom3.className = "col";
			feedDom3.src = feedItem.image;
			feedDom3.style.maxHeight = '300px';
			feedDom3.style.maxWidth = '300px';
			// feedDom3.innerText = feedItem.image;
			document.getElementById('feed-items').appendChild(feedDom3);

			const feedDom4 = document.createElement('div');
			feedDom4.className = "opacity-25";
			// const starttime = feedItem.start.split('T');
			// const time_start =  starttime[1].split('.');
			// feedDom4.innerText = "Date: "+ starttime[0] +" Time:"+ time_start[0];
			feedDom4.innerText = feedItem.start;
			document.getElementById('feed-items').appendChild(feedDom4);

			const feedDom5 = document.createElement('div');
			feedDom5.className = "opacity-75";
			// feedDom5.style.border = '2px solid #000';
			feedDom5.innerText = feedItem.description;
			document.getElementById('feed-items').appendChild(feedDom5);

			const like_ = document.createElement('div');
			like_.className = "col btn";
			like_.innerText = "Likes👍:"+feedItem.likes.length;
			like_.setAttribute("id", `${feedItem.id}-likes-id`);
			document.getElementById('feed-items').appendChild(like_);

			//function click like
			clicklike(`${feedItem.id}-likes-id`, feedItem.id);


			const likeusers = document.createElement('button');
			likeusers.className = "button";
			likeusers.setAttribute("id", `${feedItem.id}-likeusers-id`);
			likeusers.setAttribute("type", "button");
			likeusers.style.maxHeight = '30px';
			likeusers.style.maxWidth = '100px';
			likeusers.innerText = "List likes"
			document.getElementById('feed-items').appendChild(likeusers);


			const hidelikeusers = document.createElement('button');
			hidelikeusers.className = "hide button";
			hidelikeusers.setAttribute("id", `${feedItem.id}-hide-likeusers-id`);
			hidelikeusers.setAttribute("type", "button");
			hidelikeusers.style.maxHeight = '30px';
			hidelikeusers.style.maxWidth = '100px';
			hidelikeusers.innerText = "Hide likes";
			// document.getElementById('hide-likeusers-id').classList.add('hide');
			document.getElementById('feed-items').appendChild(hidelikeusers);


			listlikeusers(feedItem,`${feedItem.id}-likeusers-id`,`${feedItem.id}-hide-likeusers-id`);
			//console.log('A');
			hidelistlikeuser(feedItem,`${feedItem.id}-likeusers-id`,`${feedItem.id}-hide-likeusers-id`);




			const comments_ = document.createElement('div');
			comments_.className = "col";
			comments_.innerText = "Commnets🍔:" + feedItem.comments.length;
			comments_.setAttribute("id", `${feedItem.id}-comments-id`);			
			document.getElementById('feed-items').appendChild(comments_);	


			const writecommentbutton = document.createElement('button');
			writecommentbutton.className = "py-2 px-4 text-bg-white border border-dark rounded-pill";
			writecommentbutton.setAttribute("id", `${feedItem.id}-writecommentbutton-id`);	
			writecommentbutton.innerText = "° ° °";
			document.getElementById('feed-items').appendChild(writecommentbutton);	

			writeComment(`${feedItem.id}-writecommentbutton-id`, feedItem.id);

			const commentusers = document.createElement('button');
			commentusers.className ="button";
			commentusers.setAttribute("id", `${feedItem.id}-commentusers-id`);
			commentusers.setAttribute("type", "button");
			commentusers.style.maxHeight = '50px';
			commentusers.style.maxWidth = '100px';
			commentusers.innerText = "Show Comments";
			document.getElementById('feed-items').appendChild(commentusers);


			const hidecommentusers = document.createElement('button');
			hidecommentusers.className ="hide button";
			hidecommentusers.setAttribute("id", `${feedItem.id}-hidecommentusers-id`);
			hidecommentusers.setAttribute("type", "button");
			hidecommentusers.style.maxHeight = '50px';
			hidecommentusers.style.maxWidth = '100px';
			hidecommentusers.innerText = "Hide Comments";
			document.getElementById('feed-items').appendChild(hidecommentusers);

			showcommentusers(feedItem,`${feedItem.id}-commentusers-id`,`${feedItem.id}-hidecommentusers-id` );
			hidelistcommentusers(feedItem,`${feedItem.id}-commentusers-id`,`${feedItem.id}-hidecommentusers-id`);


			const showlikeuserlist = document.createElement('div');
			showlikeuserlist.setAttribute("id", `${feedItem.id}-showlikeuserlist-id`);
			document.getElementById('feed-items').appendChild(showlikeuserlist);


			const showcommentlist = document.createElement('div');
			showcommentlist.setAttribute("id", `${feedItem.id}-showcommentlist-id`);
			document.getElementById('feed-items').appendChild(showcommentlist);



		}
		//.then(()=>{
			//apiCall();
		//}
		//console.log('data', data);
	});
};

//*******************************************************************************
const setToken = (token) =>{
	localStorage.setItem('token', token);
	//let count = 0;
	show('section-logged-in');
	hide('section-logged-out');	
	hide('nav-login');
	hide('nav-register');
	// show('nav-home');
	show('nav-profile');
	//let count = 0;	
	//changepage(count);
	populateFeed_second();	
}

const setuserId = (userId) =>{
	localStorage.setItem('userId', userId);
	//let count = 0;

}

//CHANGE IT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
const createTime = (feedItem)=>{
			const date_ = feedItem.createdAt;

			//DEFINE A FUNCTION HERE

			// console.log(new Date());
			const date_current = new Date();
			const yearcurrent = parseInt(date_current.getFullYear());
			const monthcurrent = parseInt(date_current.getMonth());
			const daycurrent = parseInt(date_current.getDate());

			const hourcurrent = parseInt(date_current.getHours());
			const mincurrent = parseInt(date_current.getMinutes());
			const seccurrent = parseInt(date_current.getSeconds());


			const createdate = feedItem.createdAt.split('T');
			const createymd = createdate[0].split('-');
			const createtime = createdate[1].split(':');

			const year_create = parseInt(createymd[0]);
			const month_create = parseInt(createymd[1]);
			const day_create = parseInt(createymd[2]);

			const hour_create = parseInt(createtime[0]);
			const minu_create = parseInt(createtime[1]);
			const second_create = parseInt(createtime[2]);


			
			const dateText = document.createElement('div');

			if(yearcurrent - year_create> 0){
				dateText.innerText = "Created at " + day_create + "/" + month_create + "/" +year_create;
			}else if(monthcurrent - month_create > 0){
				dateText.innerText = "Created at " + day_create + "/" + month_create + "/" +year_create;					
			}else if( daycurrent - day_create >= 2){
				dateText.innerText = "Created at " + day_create + "/" + month_create + "/" +year_create;							
			}else if(daycurrent - day_create === 1){
				if((24- hour_create + hourcurrent) >= 24){
					dateText.innerText = "Created at " + day_create + "/" + month_create + "/" +year_create;	
				}else{
					dateText.innerText = "Created at " + (24- hour_create + hourcurrent) + "h " +  (60 - minu_create + mincurrent) + "min" + " ago";	
				}
			}else{//same day
				if(hourcurrent - hour_create === 0){


					dateText.innerText = "Created at " + 0 + "h " + (mincurrent- minu_create) + "min" + " ago";						

				}else if(mincurrent + minu_create<60){

					dateText.innerText = "Created at " + (hourcurrent - hour_create) + "h " + (60 - minu_create + mincurrent) + "min" + " ago";	
				}else {
					dateText.innerText = "Created at " + (hourcurrent - hour_create) + "h " + (mincurrent) + "min" + " ago";	
				}
			}
			document.getElementById('feed-items').appendChild(dateText);


}


//Package those createElement%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
const functionbody = (data)=>{
		document.getElementById('feed-items').textContent ='';
		for(const feedItem of data){

			const feedDom = document.createElement('div');
			feedDom.className = "p-3 mb-2 bg-body-tertiary bg-info-subtle text-emphasis-info";
			feedDom.innerText = feedItem.title;
			document.getElementById('feed-items').appendChild(feedDom);

			const labelName = document.createElement('label');
			labelName.className = "form-label p-3 mb-2 ";


			//job creator
			apiCall(`user?userId=${feedItem.creatorId}`, 'GET', {}, (datauser)=>{
				// console.log('userdata',datauser);

				if(datauser.id === feedItem.creatorId){
					labelName.innerText = "Job made by "; 						
					const clicknametoprofile_1 = document.createElement('button');
					clicknametoprofile_1.setAttribute("id",`${datauser.name}-clicknametoprofile-id` );
					clicknametoprofile_1.className = "btn";
					clicknametoprofile_1.setAttribute("onclick","clickUserNameToProfile('" + `${datauser.name}-clicknametoprofile-id`+"','"+`${feedItem.creatorId}`+ "')");

					clicknametoprofile_1.innerText = datauser.name;
					labelName.appendChild(clicknametoprofile_1);
				}

			});
			document.getElementById('feed-items').appendChild(labelName);

			//date and interval
			const date_ = feedItem.createdAt;

			createTime(feedItem);

			//title , id , image....
			// console.log(feedItem);

			const feedDom2 = document.createElement('div');
			feedDom2.className = "text-primary col";
			feedDom2.innerText = "id: " + feedItem.id;
			document.getElementById('feed-items').appendChild(feedDom2);

			const feedDom3 = document.createElement('img');
			feedDom3.className = "col";
			feedDom3.src = feedItem.image;
			feedDom3.style.maxHeight = '300px';
			feedDom3.style.maxWidth = '300px';
			// feedDom3.innerText = feedItem.image;
			document.getElementById('feed-items').appendChild(feedDom3);

			const feedDom4 = document.createElement('div');
			feedDom4.className = "opacity-25";
			// const starttime = feedItem.start.split('T');
			// const time_start =  starttime[1].split('.');
			// feedDom4.innerText = "Date: "+ starttime[0] +" Time:"+ time_start[0];
			feedDom4.innerText = feedItem.start;
			document.getElementById('feed-items').appendChild(feedDom4);

			const feedDom5 = document.createElement('div');
			feedDom5.className = "opacity-75";
			// feedDom5.style.border = '2px solid #000';
			feedDom5.innerText = feedItem.description;
			document.getElementById('feed-items').appendChild(feedDom5);

			const like_ = document.createElement('div');
			like_.className = "col btn";
			like_.innerText = "Likes👍:"+feedItem.likes.length;
			like_.setAttribute("id", `${feedItem.id}-likes-id`);
			document.getElementById('feed-items').appendChild(like_);

			//function click like
			clicklike(`${feedItem.id}-likes-id`, feedItem.id);


			const likeusers = document.createElement('button');
			likeusers.className = "py-2 px-4 text-bg-dark border border-white rounded-pill";
			likeusers.setAttribute("id", `${feedItem.id}-likeusers-id`);
			likeusers.setAttribute("type", "button");
			likeusers.style.maxHeight = '32px';
			likeusers.style.maxWidth = '80px';
			likeusers.innerText = "° ° °"
			document.getElementById('feed-items').appendChild(likeusers);


			const hidelikeusers = document.createElement('button');
			hidelikeusers.className = "hide py-2 px-4 text-bg-white border border-white rounded-pill";
			hidelikeusers.setAttribute("id", `${feedItem.id}-hide-likeusers-id`);
			hidelikeusers.setAttribute("type", "button");
			hidelikeusers.style.maxHeight = '32px';
			hidelikeusers.style.maxWidth = '80px';
			hidelikeusers.innerText = "° ° °";
			// document.getElementById('hide-likeusers-id').classList.add('hide');
			document.getElementById('feed-items').appendChild(hidelikeusers);


			listlikeusers(feedItem,`${feedItem.id}-likeusers-id`,`${feedItem.id}-hide-likeusers-id`);
			//console.log('A');
			hidelistlikeuser(feedItem,`${feedItem.id}-likeusers-id`,`${feedItem.id}-hide-likeusers-id`);

			const comments_ = document.createElement('div');
			comments_.className = "col";
			comments_.innerText = "Commnets🍔:" + feedItem.comments.length;
			comments_.setAttribute("commets", `${feedItem.id}-comments-id`);			
			document.getElementById('feed-items').appendChild(comments_);	

			const writecommentInput = document.createElement('textarea');
			writecommentInput.className = "form-control"
			writecommentInput.setAttribute("id",`${feedItem.id}-writecommentInput-id` );
			writecommentInput.setAttribute("placeholder", "This is a great opportunity");
			writecommentInput.innerText = "";


			const writecommentbutton = document.createElement('button');
			writecommentbutton.className = "btn btn-outline-secondary";
			writecommentbutton.setAttribute("id", `${feedItem.id}-writecommentbutton-id`);	
			writecommentbutton.innerText = "submit";
			//document.getElementById('feed-items').appendChild(writecommentbutton);	

			const writecommentinputfield = document.createElement('div');
			writecommentinputfield.setAttribute("id", `${feedItem.id}-writecommentinputfield-id`);
			writecommentinputfield.className = "hide input-group mb-3";
			writecommentinputfield.appendChild(writecommentInput);
			writecommentinputfield.appendChild(writecommentbutton);


			const commentusers = document.createElement('button');
			commentusers.className ="py-2 px-4 text-bg-dark border border-white rounded-pill";
			commentusers.setAttribute("id", `${feedItem.id}-commentusers-id`);
			commentusers.setAttribute("type", "button");
			commentusers.style.maxHeight = '32px';
			commentusers.style.maxWidth = '80px';
			commentusers.innerText = "° ° °";
			document.getElementById('feed-items').appendChild(commentusers);


			const hidecommentusers = document.createElement('button');
			hidecommentusers.className ="hide py-2 px-4 text-bg-white border border-white rounded-pill";
			hidecommentusers.setAttribute("id", `${feedItem.id}-hidecommentusers-id`);
			hidecommentusers.setAttribute("type", "button");
			hidecommentusers.style.maxHeight = '32px';
			hidecommentusers.style.maxWidth = '80px';
			hidecommentusers.innerText = "° ° °";
			document.getElementById('feed-items').appendChild(hidecommentusers);

			showcommentusers(feedItem,`${feedItem.id}-commentusers-id`,`${feedItem.id}-hidecommentusers-id` );
			hidelistcommentusers(feedItem,`${feedItem.id}-commentusers-id`,`${feedItem.id}-hidecommentusers-id`);


			const showlikeuserlist = document.createElement('div');
			showlikeuserlist.setAttribute("id", `${feedItem.id}-showlikeuserlist-id`);
			document.getElementById('feed-items').appendChild(showlikeuserlist);


			const showcommentlist = document.createElement('div');
			showcommentlist.setAttribute("id", `${feedItem.id}-showcommentlist-id`);
			showcommentlist.appendChild(writecommentinputfield);
			document.getElementById('feed-items').appendChild(showcommentlist);



}
}


const functionbody_i = (feedItem, div_i)=>{
		//document.getElementById(div_i).textContent ='';


			const feedDom = document.createElement('div');
			feedDom.className = "p-3 mb-2 bg-body-tertiary bg-info-subtle text-emphasis-info";
			feedDom.innerText = feedItem.title;
			div_i.appendChild(feedDom);

			const labelName = document.createElement('label');
			labelName.className = "form-label p-3 mb-2 ";


			//job creator
			apiCall(`user?userId=${feedItem.creatorId}`, 'GET', {}, (datauser)=>{
				// console.log('userdata',datauser);

				if(datauser.id === feedItem.creatorId){
					labelName.innerText = "Job made by " + datauser.name; 
					labelName.setAttribute("id", `${feedItem.id}-labelName-id`);
					labelName.setAttribute("onclick","clickUserNameToProfile('" + `${feedItem.id}-labelName-id`+"','"+`${feedItem.creatorId}`+ "')")
					
				}

			});
			div_i.appendChild(labelName);

			//date and interval
			const date_ = feedItem.createdAt;

			createTime(feedItem);

			//title , id , image....
			// console.log(feedItem);

			const feedDom2 = document.createElement('div');
			feedDom2.className = "text-primary col";
			feedDom2.innerText = "id: " + feedItem.id;
			div_i.appendChild(feedDom2);

			const feedDom3 = document.createElement('img');
			feedDom3.className = "col";
			feedDom3.src = feedItem.image;
			feedDom3.style.maxHeight = '300px';
			feedDom3.style.maxWidth = '300px';
			// feedDom3.innerText = feedItem.image;
			div_i.appendChild(feedDom3);

			const feedDom4 = document.createElement('div');
			feedDom4.className = "opacity-25";
			// const starttime = feedItem.start.split('T');
			// const time_start =  starttime[1].split('.');
			// feedDom4.innerText = "Date: "+ starttime[0] +" Time:"+ time_start[0];
			feedDom4.innerText = feedItem.start;
			div_i.appendChild(feedDom4);

			const feedDom5 = document.createElement('div');
			feedDom5.className = "opacity-75";
			// feedDom5.style.border = '2px solid #000';
			feedDom5.innerText = feedItem.description;
			div_i.appendChild(feedDom5);

			const like_ = document.createElement('div');
			like_.className = "col btn";
			like_.innerText = "Likes👍:"+feedItem.likes.length;
			like_.setAttribute("id", `${feedItem.id}-likes-id`);
			div_i.appendChild(like_);

			//function click like
			clicklike(`${feedItem.id}-likes-id`, feedItem.id);


			const likeusers = document.createElement('button');
			likeusers.className = "py-2 px-4 text-bg-dark border border-white rounded-pill";
			likeusers.setAttribute("id", `${feedItem.id}-likeusers-id`);
			likeusers.setAttribute("type", "button");
			likeusers.style.maxHeight = '32px';
			likeusers.style.maxWidth = '80px';
			likeusers.innerText = "° ° °"
			div_i.appendChild(likeusers);


			const hidelikeusers = document.createElement('button');
			hidelikeusers.className = "hide py-2 px-4 text-bg-white border border-white rounded-pill";
			hidelikeusers.setAttribute("id", `${feedItem.id}-hide-likeusers-id`);
			hidelikeusers.setAttribute("type", "button");
			hidelikeusers.style.maxHeight = '32px';
			hidelikeusers.style.maxWidth = '80px';
			hidelikeusers.innerText = "° ° °";
			// document.getElementById('hide-likeusers-id').classList.add('hide');
			div_i.appendChild(hidelikeusers);


			listlikeusers(feedItem,`${feedItem.id}-likeusers-id`,`${feedItem.id}-hide-likeusers-id`);
			//console.log('A');
			hidelistlikeuser(feedItem,`${feedItem.id}-likeusers-id`,`${feedItem.id}-hide-likeusers-id`);

			const comments_ = document.createElement('div');
			comments_.className = "col";
			comments_.innerText = "Commnets🍔:" + feedItem.comments.length;
			comments_.setAttribute("commets", `${feedItem.id}-comments-id`);			
			div_i.appendChild(comments_);	

			const writecommentInput = document.createElement('textarea');
			writecommentInput.className = "form-control"
			writecommentInput.setAttribute("id",`${feedItem.id}-writecommentInput-id` );
			writecommentInput.setAttribute("placeholder", "This is a great opportunity");
			writecommentInput.innerText = "";


			const writecommentbutton = document.createElement('button');
			writecommentbutton.className = "btn btn-outline-secondary";
			writecommentbutton.setAttribute("id", `${feedItem.id}-writecommentbutton-id`);	
			writecommentbutton.innerText = "submit";
			//div_i.appendChild(writecommentbutton);	

			const writecommentinputfield = document.createElement('div');
			writecommentinputfield.setAttribute("id", `${feedItem.id}-writecommentinputfield-id`);
			writecommentinputfield.className = "hide input-group mb-3";
			writecommentinputfield.appendChild(writecommentInput);
			writecommentinputfield.appendChild(writecommentbutton);


			const commentusers = document.createElement('button');
			commentusers.className ="py-2 px-4 text-bg-dark border border-white rounded-pill";
			commentusers.setAttribute("id", `${feedItem.id}-commentusers-id`);
			commentusers.setAttribute("type", "button");
			commentusers.style.maxHeight = '32px';
			commentusers.style.maxWidth = '80px';
			commentusers.innerText = "° ° °";
			div_i.appendChild(commentusers);


			const hidecommentusers = document.createElement('button');
			hidecommentusers.className ="hide py-2 px-4 text-bg-white border border-white rounded-pill";
			hidecommentusers.setAttribute("id", `${feedItem.id}-hidecommentusers-id`);
			hidecommentusers.setAttribute("type", "button");
			hidecommentusers.style.maxHeight = '32px';
			hidecommentusers.style.maxWidth = '80px';
			hidecommentusers.innerText = "° ° °";
			div_i.appendChild(hidecommentusers);

			showcommentusers(feedItem,`${feedItem.id}-commentusers-id`,`${feedItem.id}-hidecommentusers-id` );
			hidelistcommentusers(feedItem,`${feedItem.id}-commentusers-id`,`${feedItem.id}-hidecommentusers-id`);


			const showlikeuserlist = document.createElement('div');
			showlikeuserlist.setAttribute("id", `${feedItem.id}-showlikeuserlist-id`);
			div_i.appendChild(showlikeuserlist);


			const showcommentlist = document.createElement('div');
			showcommentlist.setAttribute("id", `${feedItem.id}-showcommentlist-id`);
			showcommentlist.appendChild(writecommentinputfield);
			div_i.appendChild(showcommentlist);

}


const functionbody_ii = (data, profile_items_id)=>{
		document.getElementById(profile_items_id).textContent ='';

		const userprofileinfotable = document.createElement('div');
		userprofileinfotable.className = "table-primary";

		const userprofileemail = document.createElement('div');
		userprofileemail.setAttribute("scope", "col");
		userprofileemail.innerText = "Email: " + data.email;
		userprofileinfotable.appendChild(userprofileemail);

		const userprofileName = document.createElement('div');
		userprofileName.setAttribute("scope", "col");
		userprofileName.innerText = "Name: " + data.name;
		userprofileinfotable.appendChild(userprofileName);

		const userprofileImage = document.createElement('div');
		userprofileImage.setAttribute("scope", "col");
		userprofileImage.innerText = "Image: " + data.image;
		userprofileinfotable.appendChild(userprofileImage);


		const userprofileWatch = document.createElement('div');
		userprofileWatch.setAttribute("scope", "col");
		userprofileWatch.innerText = "Who watch: ";
		for(const each of data.watcheeUserIds){

			const userprofileWatch_each = document.createElement('li');
			userprofileWatch_each.style.display = "inline";
			apiCall(`user?userId=${each}`, 'GET', {}, (datauser)=>{
					userprofileWatch_each.innerText = datauser.name;

			});
			userprofileWatch.appendChild(userprofileWatch_each);

		}

		userprofileinfotable.appendChild(userprofileWatch);

		document.getElementById(profile_items_id).appendChild(userprofileinfotable);

		for(const feedItem of data.jobs){

			const feedDom = document.createElement('div');
			feedDom.className = "p-3 mb-2 bg-body-tertiary bg-info-subtle text-emphasis-info";
			feedDom.innerText = feedItem.title;
			document.getElementById(profile_items_id).appendChild(feedDom);

			const labelName = document.createElement('label');
			labelName.className = "form-label p-3 mb-2 ";


			//job creator
			apiCall(`user?userId=${feedItem.creatorId}`, 'GET', {}, (datauser)=>{
				// console.log('userdata',datauser);

				if(datauser.id === feedItem.creatorId){
					labelName.innerText = "Job made by "; 						
					const clicknametoprofile_1 = document.createElement('button');
					clicknametoprofile_1.setAttribute("id",`${datauser.name}-clicknametoprofile-id` );
					clicknametoprofile_1.setAttribute("onclick","clickUserNameToProfile('" + `${datauser.name}-clicknametoprofile-id`+"','"+`${feedItem.creatorId}`+ "')")
					clicknametoprofile_1.className = "btn";
					clicknametoprofile_1.innerText = datauser.name;
					labelName.appendChild(clicknametoprofile_1);
				}

			});
			document.getElementById(profile_items_id).appendChild(labelName);

			//date and interval
			const date_ = feedItem.createdAt;

			createTime(feedItem);

			//title , id , image....
			// console.log(feedItem);

			const feedDom2 = document.createElement('div');
			feedDom2.className = "text-primary col";
			feedDom2.innerText = "id: " + feedItem.id;
			document.getElementById(profile_items_id).appendChild(feedDom2);

			const feedDom3 = document.createElement('img');
			feedDom3.className = "col";
			feedDom3.src = feedItem.image;
			feedDom3.style.maxHeight = '300px';
			feedDom3.style.maxWidth = '300px';
			// feedDom3.innerText = feedItem.image;
			document.getElementById(profile_items_id).appendChild(feedDom3);

			const feedDom4 = document.createElement('div');
			feedDom4.className = "opacity-25";
			// const starttime = feedItem.start.split('T');
			// const time_start =  starttime[1].split('.');
			// feedDom4.innerText = "Date: "+ starttime[0] +" Time:"+ time_start[0];
			feedDom4.innerText = feedItem.start;
			document.getElementById(profile_items_id).appendChild(feedDom4);

			const feedDom5 = document.createElement('div');
			feedDom5.className = "opacity-75";
			// feedDom5.style.border = '2px solid #000';
			feedDom5.innerText = feedItem.description;
			document.getElementById(profile_items_id).appendChild(feedDom5);

			const like_ = document.createElement('div');
			like_.className = "col btn";
			like_.innerText = "Likes👍:"+feedItem.likes.length;
			like_.setAttribute("id", `${feedItem.id}-likes-id`);
			document.getElementById(profile_items_id).appendChild(like_);

			//function click like
			clicklike(`${feedItem.id}-likes-id`, feedItem.id);


			const likeusers = document.createElement('button');
			likeusers.className = "py-2 px-4 text-bg-dark border border-white rounded-pill";
			likeusers.setAttribute("id", `${feedItem.id}-likeusers-id`);
			likeusers.setAttribute("type", "button");
			likeusers.style.maxHeight = '32px';
			likeusers.style.maxWidth = '80px';
			likeusers.innerText = "° ° °";
			document.getElementById(profile_items_id).appendChild(likeusers);


			const hidelikeusers = document.createElement('button');
			hidelikeusers.className = "hide py-2 px-4 text-bg-white border border-white rounded-pill";
			hidelikeusers.setAttribute("id", `${feedItem.id}-hide-likeusers-id`);
			hidelikeusers.setAttribute("type", "button");
			hidelikeusers.style.maxHeight = '32px';
			hidelikeusers.style.maxWidth = '80px';
			hidelikeusers.innerText = "° ° °";
			// document.getElementById('hide-likeusers-id').classList.add('hide');
			document.getElementById(profile_items_id).appendChild(hidelikeusers);


			listlikeusers(feedItem,`${feedItem.id}-likeusers-id`,`${feedItem.id}-hide-likeusers-id`);
			//console.log('A');
			hidelistlikeuser(feedItem,`${feedItem.id}-likeusers-id`,`${feedItem.id}-hide-likeusers-id`);

			const comments_ = document.createElement('div');
			comments_.className = "col";
			comments_.innerText = "Commnets🍔:" + feedItem.comments.length;
			comments_.setAttribute("commets", `${feedItem.id}-comments-id`);			
			document.getElementById(profile_items_id).appendChild(comments_);	

			const writecommentInput = document.createElement('textarea');
			writecommentInput.className = "form-control";
			writecommentInput.setAttribute("id",`${feedItem.id}-writecommentInput-id` );
			writecommentInput.setAttribute("placeholder", "This is a great opportunity");
			writecommentInput.innerText = "";


			const writecommentbutton = document.createElement('button');
			writecommentbutton.className = "btn btn-outline-secondary";
			writecommentbutton.setAttribute("id", `${feedItem.id}-writecommentbutton-id`);	
			writecommentbutton.innerText = "submit";
			//document.getElementById(profile_items_id).appendChild(writecommentbutton);	

			const writecommentinputfield = document.createElement('div');
			writecommentinputfield.setAttribute("id", `${feedItem.id}-writecommentinputfield-id`);
			writecommentinputfield.className = "hide input-group mb-3";
			writecommentinputfield.appendChild(writecommentInput);
			writecommentinputfield.appendChild(writecommentbutton);


			const commentusers = document.createElement('button');
			commentusers.className ="py-2 px-4 text-bg-dark border border-white rounded-pill";
			commentusers.setAttribute("id", `${feedItem.id}-commentusers-id`);
			commentusers.setAttribute("type", "button");
			commentusers.style.maxHeight = '32px';
			commentusers.style.maxWidth = '80px';
			commentusers.innerText = "° ° °";
			document.getElementById(profile_items_id).appendChild(commentusers);


			const hidecommentusers = document.createElement('button');
			hidecommentusers.className ="hide py-2 px-4 text-bg-white border border-white rounded-pill";
			hidecommentusers.setAttribute("id", `${feedItem.id}-hidecommentusers-id`);
			hidecommentusers.setAttribute("type", "button");
			hidecommentusers.style.maxHeight = '32px';
			hidecommentusers.style.maxWidth = '80px';
			hidecommentusers.innerText = "° ° °";
			document.getElementById(profile_items_id).appendChild(hidecommentusers);

			showcommentusers(feedItem,`${feedItem.id}-commentusers-id`,`${feedItem.id}-hidecommentusers-id` );
			hidelistcommentusers(feedItem,`${feedItem.id}-commentusers-id`,`${feedItem.id}-hidecommentusers-id`);


			const showlikeuserlist = document.createElement('div');
			showlikeuserlist.setAttribute("id", `${feedItem.id}-showlikeuserlist-id`);
			document.getElementById(profile_items_id).appendChild(showlikeuserlist);


			const showcommentlist = document.createElement('div');
			showcommentlist.setAttribute("id", `${feedItem.id}-showcommentlist-id`);
			showcommentlist.appendChild(writecommentinputfield);
			document.getElementById(profile_items_id).appendChild(showcommentlist);



}
}


//polling
// 	const poll=(path,method,payload)=>{
// 			setTimeout(()=>{
// 			const options = {
// 				method: method,
// 				headers:{
// 					'Content-type': 'application/json',
// 				},
// 				body: JSON.stringify(payload)
// 			};

// 			if(localStorage.getItem('token')){
// 				options.headers.Authorization = `Bearer ${localStorage.getItem('token')}`;
// 			}

// 			fetch('http://localhost:5005/'+path, options)
// 			.then(response => {
// 				if(response.status === 404){
// 		        	popupAlert('It\'s 404');
// 				}

// 						response.json()
// 							.then((data)=>{
// 								poll();
// 						}).catch(error=>{
// 							console.log(error);

// 							poll();
// 						});
// 					});

// 			//end of interval
// 		},5000);
// 	}

//click like job
const clicklike = (like_id, job_id)=>{


	document.getElementById(like_id).addEventListener('click', ()=>{

			const payload={
				"id": job_id,
  			"turnon": true
			};	

			apiCall('job/like', 'PUT', payload);
			//poll("job/like",'PUT', payload);	

	});


}

const writeComment = (comment_button_id,job_id, comment_input_id)=>{

	document.getElementById(comment_button_id).addEventListener('click', ()=>{
	const comment = document.getElementById(comment_input_id).value;
		const payload={
			"id": job_id,
			"comment" : comment
		}
		apiCall('job/comment', 'POST', payload);
	});
	
}








//list all users who like
const listlikeusers = (feedItem_,id_users, id_hide_users)=>{
	// console.log("call listlikeusers functions");
	document.getElementById(id_users).addEventListener('click',()=>{
		// console.log("click call like user list");
			const userlist = document.createElement('div');
			userlist.setAttribute("id", `${feedItem_.id}-userlist-id`);
			userlist.innerText = "Who like this job: ";
			userlist.className = "col";

			for(const eachusers of feedItem_.likes){
				console.log(feedItem_.id,eachusers);
				const userlist_li = document.createElement('li');
				userlist_li.className = "btn";
				userlist_li.setAttribute("id", `${eachusers.userName}-id`);
				userlist_li.setAttribute("value", `${eachusers.userId}`);
				userlist_li.innerText = "😃"+ eachusers.userName + "  ";
				userlist_li.style.display = "inline";

				userlist.appendChild(userlist_li);

				userlist_li.setAttribute("onclick", "clickUserNameToProfile('" + `${eachusers.userName}-id` +"','"+`${eachusers.userId}`+ "')");


				//console.log(userlist_li.value);
			}

			document.getElementById(`${feedItem_.id}-showlikeuserlist-id`).appendChild(userlist);

			hide(id_users);
			show(id_hide_users);
	});
}

//hide all users who like
const hidelistlikeuser = (feedItem_,id_users, id_hide_users)=>{

	document.getElementById(id_hide_users).addEventListener('click',()=>{
		const showlikeuser_div = document.getElementById(`${feedItem_.id}-showlikeuserlist-id`);
		// console.log("click call hide like user list button");
		const userlist = document.getElementById(`${feedItem_.id}-userlist-id`);
			for(const eachusers of feedItem_.likes){
				const userlist_li = document.getElementById(`${eachusers.userName}-id`);
				userlist.removeChild(userlist_li);
			}
		// while()
	showlikeuser_div.removeChild(userlist);
	hide(id_hide_users);
	show(id_users);
	});

}


//show comments
const showcommentusers = (feedItem_,id_users, id_hide_users, button_id)=>{
	document.getElementById(id_users).addEventListener('click', ()=>{

	writeComment(`${feedItem_.id}-writecommentbutton-id`, feedItem_.id,`${feedItem_.id}-writecommentInput-id`);
	const wf = document.getElementById(`${feedItem_.id}-writecommentinputfield-id`).classList.remove('hide');

		console.log("click call comment user list"); 
		const clist = document.createElement('table'); //for put user name and user comments
		//user name and user comment
		clist.className = "table table-striped";
		clist.setAttribute("id", `${feedItem_.id}-clist`);
		const title_ = document.createElement('tr');
		title_.setAttribute("id",`${feedItem_.id}-title_`);
		const title_username = document.createElement('th');
		title_username.innerText = "Users";
		title_username.setAttribute("id",`${feedItem_.id}-title_username`);
		const title_comment = document.createElement('th');
		title_comment.innerText = "Comments";
		title_comment.setAttribute("id",`${feedItem_.id}-title_comment`);

		//title_.appendChild(title_username);
		title_.appendChild(title_comment);
		clist.appendChild(title_);

		for(const eachcomments of feedItem_.comments){
			const main_content = document.createElement('tr');
			main_content.setAttribute("id", `${eachcomments.userId}-main_content-id`);

			//user name
			const comment_user_list = document.createElement('th');
			comment_user_list.setAttribute("id", `${eachcomments.userId}-comment_user_list`);
			comment_user_list.setAttribute("value", `${eachcomments.userId}`);
			comment_user_list.className = "btn";
			comment_user_list.innerText = eachcomments.userName + ":";
			//clickUserNameToProfile(`${eachcomments.userId}-comment_user_list`);
			comment_user_list.setAttribute("onclick", "clickUserNameToProfile('" + `${eachcomments.userId}-comment_user_list` +"','"+`${eachcomments.userId}`+ "')");

			//comments
			const comment_itself_list = document.createElement('th');	
			comment_itself_list.setAttribute("id", `${eachcomments.userId}-comment_itself_list`);
					comment_itself_list.className = "col";
			comment_itself_list.innerText = eachcomments.comment;

			main_content.appendChild(comment_user_list);
			main_content.appendChild(comment_itself_list);
			clist.appendChild(main_content);

		}

		//clist.appendChild(main_content);
		document.getElementById(`${feedItem_.id}-showcommentlist-id`).appendChild(clist);

		hide(id_users);
		show(id_hide_users);


	});
}


//hide comments
const hidelistcommentusers = (feedItem_,id_users, id_hide_users,button_id)=>{

	document.getElementById(id_hide_users).addEventListener('click',()=>{
		const wf = document.getElementById(`${feedItem_.id}-writecommentinputfield-id`).classList.add('hide');
		console.log("click hide comment user list"); 
			const clist = document.getElementById(`${feedItem_.id}-clist`);

			const title_= document.getElementById(`${feedItem_.id}-title_`);
			const title_username = document.getElementById(`${feedItem_.id}-title_username`);
			const title_comment = document.getElementById(`${feedItem_.id}-title_comment`);

			for(const eachcomments of feedItem_.comments){
			const main_content = document.getElementById(`${eachcomments.userId}-main_content-id`);
			const comment_user_list = document.getElementById(`${eachcomments.userId}-comment_user_list`);
			const comment_itself_list = document.getElementById( `${eachcomments.userId}-comment_itself_list`);

			main_content.removeChild(comment_user_list);
			main_content.removeChild(comment_itself_list);	
			clist.removeChild(main_content);			
			}

			//title_.removeChild(title_username);
			title_.removeChild(title_comment);
			clist.removeChild(title_);
			document.getElementById(`${feedItem_.id}-showcommentlist-id`).removeChild(clist);

			hide(id_hide_users);
			show(id_users);

	});

}


document.getElementById('nav-profile').addEventListener('click',()=>{

	console.log("click nav-profile");
			const id = localStorage.getItem('userId');
			const payload={

			};
			apiCall('user?userId='+`${id}`, 'GET', payload, (data)=>{

			// console.log(data);

			functionbody_ii(data,'profile-items');
			// show(id_users);


		});

		// hidelistcommentusers();
		hide('section-logged-in');
		show('section-user-profile');
		show('goto-update-profile');

});


let turnon_i = false;
let turnonemail = "";
//click name to profile
window.clickUserNameToProfile = function (userName_id, id_){

	//document.getElementById(userName_id).addEventListener('click',()=>{
		//show('watch-user-profile');


		//const turnon_i = false;


		console.log(userName_id);
		console.log(id_);
		const user_id = document.getElementById(userName_id).value;
		//console.log(user_id);
		const payload ={
			id: id_
		};

		// const payload_i = {

		// };
		// apiCall('user/watch','PUT',payload_i,(data)=>{
		// 	//turnon_i = data.turnon;

		// 	window.turnon_i = data.turnon; 
		// 	console.log(turnon_i);

		// 	//return JOSN.stringify(data.turnon);
		// });

		apiCall('user?userId='+`${id_}`, 'GET', payload, (data)=>{

			// console.log(data);

			functionbody_ii(data,'profile-items');
			// window.turnonemail = data.watcheeUserIds.

			// show(id_users);
			


		});

		apiCall('user?userId='+`${localStorage.getItem('userId')}`, 'GET', (data)=>{

			window.turnonemail = data.email;

		});

		apiCall('user/watch','PUT',{},(data)=>{
					//turnon_i = data.turnon;

					if(turnonemail === data.email){
					window.turnon_i = data.turnon; 
					console.log(turnon_i);

					}

					//return JOSN.stringify(data.turnon);
				});
		
		console.log(turnon_i);

			if(turnon_i === false){
			show('watch-user-profile');
			hide('unwatch-user-profile');

			}else{
				hide('watch-user-profile');
				show('unwatch-user-profile');
			}

		// hidelistcommentusers();
		hide('section-logged-in');
		show('section-user-profile');
		//getelementByid end
	//});

}



document.getElementById('goto-update-profile').addEventListener('click',()=>{

	show('section-user-profile-update');

});




document.getElementById('profile-update-button').addEventListener('click',()=>{

	const email_ = document.getElementById('profile-update-email').value;
	const password_ = document.getElementById('profile-update-password').value;
	const name_ = document.getElementById('profile-update-name').value;
	const image_ = document.getElementById('profile-update-image').value;

	const payload={
	"email": email_,
  "password": password_,
  "name": name_,
  "image": image_

	};
	apiCall('user','PUT',payload,(data)=>{

		console.log(data);
	});
	hide('section-user-profile-update');
});



document.getElementById('watch-user-profile').addEventListener('click',()=>{
	show('watch-user-profile-email-div');
	show('pemail-1');
	hide('pemail-2');

});

let userEmail_ii = "";
document.getElementById('unwatch-user-profile').addEventListener('click',()=>{

	show('watch-user-profile-email-div');
	show('pemail-2');
	hide('pemail-1');

/*	const userId_ = localStorage.getItem('userId');
	//const userEmail_ = 0;

	const payload_i = {};

	apiCall('user?userId='+`${userId_}`,payload_i,(data)=>{

		window.userEmail_ii = data.email;

	});*/

	// const payload = {
	// "email": userEmail_ii,
 //  "turnon": false
	// };
	// apiCall('user/watch','PUT',payload,(data)=>{
		
	// });
	// show('watch-user-profile');
	// hide('unwatch-user-profile');
});



//close
document.getElementById('close-watch-user-profile-email').addEventListener('click',()=>{

	hide('watch-user-profile-email-div');

});

document.getElementById('close-unwatch-user-profile-email').addEventListener('click',()=>{

	hide('watch-user-profile-email-div');

});

//submit
document.getElementById('submit-watch-user-profile-email').addEventListener('click',()=>{

	const email_ = document.getElementById('watch-user-profile-email').value;

	const payload = {
	"email": email_,
  "turnon": true
	};
	apiCall('user/watch','PUT',payload,(data)=>{

	});

	hide('watch-user-profile-email-div');
	
	show('unwatch-user-profile');
	hide('watch-user-profile');

});

document.getElementById('submit-unwatch-user-profile-email').addEventListener('click',()=>{

	const email_ = document.getElementById('unwatch-user-profile-email').value;

	const payload = {
	"email": email_,
  "turnon": false
	};
	apiCall('user/watch','PUT',payload,(data)=>{

	});

	
	hide('watch-user-profile-email-div');
	show('watch-user-profile');
	hide('unwatch-user-profile');

});

document.getElementById("from-profile-back").addEventListener('click', ()=>{

	hide('section-user-profile');
	show('section-logged-in');
	hide('watch-user-profile');
	hide('unwatch-user-profile');

});

















//create fake job 
document.getElementById('create-job-fake').addEventListener('click', ()=>{

	const title_ = document.getElementById('job-title').value;
	const image_ = document.getElementById('job-image').value;
	const start_ = document.getElementById('job-start').value;
	const description_ = document.getElementById('job-description').value;
	const id_ = document.getElementById('job-id').value;

	 if(id_ )
	  {
	  	 popupAlert("Don't insert id for creating job");
	  }else{

		  const payload = {
		    "title": title_,
		    "image": image_,
		    "start": start_,
		    "description": description_
		  };
		  apiCall('job', 'POST', payload);
		  console.log('click create job fake');
	  }

	//populateFeed();
});

document.getElementById('update-job').addEventListener('click', ()=>{

	const title_ = document.getElementById('job-title').value;
	const image_ = document.getElementById('job-image').value;
	const start_ = document.getElementById('job-start').value;
	const description_ = document.getElementById('job-description').value;
	const id_ = document.getElementById('job-id').value;

   if(id_.toString().length !== 6)
	  {
	  	 popupAlert("Please insert correct id to update");
	  }

  const payload = {
  	"id": id_,
    "title": title_,
    "image": image_,
    "start": start_,
    "description": description_
  };
  apiCall('job', 'PUT', payload);
  console.log('click create job Update');
	//populateFeed();
});

const remove_job_func = (removejobbutton_id, job_id)=>{
	document.getElementById(removejobbutton_id).addEventListener('click', ()=>{
	  console.log('Remove job');

		const payload = {
	     "id": job_id
	  	};
	  apiCall('job', 'DELETE', payload);
	});
}

document.getElementById('remove-job').addEventListener('click', ()=>{
  console.log('Remove job');
  const id_ = document.getElementById('job-id').value;

	const payload = {
     "id": id_
  	};
  apiCall('job', 'DELETE', payload);
});

//register button click
document.getElementById(`register-button`).addEventListener('click', ()=>{

	const email_ = document.getElementById('register-email').value;
	const name_ = document.getElementById('register-name').value;
	const password_ = document.getElementById('register-password').value;
	const con_password_ = document.getElementById('register-password').value;

	//check if all input field filled
	if(email_ === '' || name_ ==='' || password_ === '' || con_password_ === ''){
		popupAlert("Please fill all the field");	
	}
	else if(con_password_ !== password_){

		popupAlert("Confirm password should be same with your expected password");	

	}else{
			const payload = {
			email: email_,
			name: name_,
			password: password_
			}	
			apiCall("auth/register",'POST',payload,(data) =>{
			//console.log('Success!',data);
			setToken(data.token);
			setuserId(data.userId);
			});
  hide('nav-login');
	hide('nav-register');
	//show('nav-home');
	show('nav-profile');
	}


});



//login button click
document.getElementById(`login-button`).addEventListener('click', ()=>{
	const payload = {
		email: document.getElementById('login-email').value,
		password: document.getElementById('login-password').value
	}
	apiCall('auth/login','POST',payload, (data) =>{
		console.log('Success!',data);
		setToken(data.token);
		setuserId(data.userId);
	});
	hide('nav-login');
	hide('nav-register');
	show('nav-profile');
});


const show = (element) =>{
	// console.log("show");
	document.getElementById(`${element}`).classList.remove('hide');

}

const hide = (element) =>{
	// console.log("hide");
	document.getElementById(`${element}`).classList.add('hide');	
}

document.getElementById('nav-register').addEventListener('click', ()=>{
	show('page-register');
	hide('page-login');
	hide('nav-home');
	hide('nav-profile');
	show('logout');
});


document.getElementById('nav-login').addEventListener('click',()=>{
	show('page-login');
	hide('page-register');
	hide('nav-home');
	hide('nav-profile');
	show('logout');
});


document.getElementById('logout').addEventListener('click',()=>{
	show('section-logged-out');
	hide('section-logged-in');
	localStorage.removeItem('token');
	show('nav-login');
	show('nav-register');
	hide('nav-home');
	hide('nav-profile');
	hide('logout');
	hide('section-user-profile');
});	


//***************************************************************************

if (localStorage.getItem('token')) {
  show('section-logged-in');
  hide('section-logged-out');
  hide('nav-login');
	hide('nav-register');
	// show('nav-home');
	show('nav-profile');
  //let count = 0;
  //changepage(count);
  populateFeed_second();
  console.log('token', localStorage.getItem('token'));
  console.log('userId',localStorage.getItem('userId'));
}




//pop up alert
// const popupAlert = (errormessage)=>{

// 	const child = document.createElement("p");
// 	const childtext = document.createTextNode(errormessage);
// 	child.appendChild(childtext);
// 	document.getElementById("popup-alert-p").appendChild(child);	

// 	document.getElementById("popup-alert").classList.remove('hide');
// }


// const closepopupAlert = (event)=>{

// 	const popalert = document.getElementById("popup-alert");

// 	popalert.classList.add('hide');

// 	const popalert_p = document.getElementById("popup-alert-p");
// 	while(popalert_p.firstChild){
// 		popalert_p.removeChild(popalert_p.lastChild);
// 	}


// }

// document.getElementById('close-alert').addEventListener('click', closepopupAlert);
















// if ('serviceWorker' in navigator) {
//   window.addEventListener('load', function() {
//     navigator.serviceWorker.register('/ServiceWorker.js').then(function(registration) {
//       
//       console.log('Service Worker registered with scope: ', registration.scope);
//     }, function(err) {
//     
//       console.log('Service Worker registration failed: ', err);
//     });
//   });
// }
