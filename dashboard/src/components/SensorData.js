import { db } from '../firebase';
import { collection, query, orderBy, limit, getDocs } from 'firebase/firestore';

export const fetchSensorData = async (sensor) => {
  const q = query(
    collection(db, sensor),
    orderBy('time', 'desc'),
    limit(1)
  );
  
  const querySnapshot = await getDocs(q);
  let data = {};
  querySnapshot.forEach((doc) => {
    data = doc.data();  // 최신 데이터만 추출
  });

  return data;
};
