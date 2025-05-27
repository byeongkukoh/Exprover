// Import the functions you need from the SDKs you need
// src/firebase.js
import { initializeApp } from "firebase/app";
import { getFirestore } from "firebase/firestore";
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
const firebaseConfig = {
  apiKey: "AIzaSyDhzIWfldJ9ejT23GpvS66LyTbCsgq-s2A",
  authDomain: "rc-car-pjt-39ec0.firebaseapp.com",
  projectId: "rc-car-pjt-39ec0",
  storageBucket: "rc-car-pjt-39ec0.firebasestorage.app",
  messagingSenderId: "393060247",
  appId: "1:393060247:web:0a0bb0914b0988a892aed2"
};

const app = initializeApp(firebaseConfig);
const db = getFirestore(app);

export { db };
