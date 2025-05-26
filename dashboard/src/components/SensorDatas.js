import { db } from '../firebase';
import { collection, query, orderBy, limit, getDocs } from 'firebase/firestore';

export const fetchRecentSensorData = async (sensor) => {
  const q = query(
    collection(db, sensor),
    orderBy('time', 'desc'),
    limit(30)
  );

  const snapshot = await getDocs(q);
  const data = [];
  snapshot.forEach(doc => data.push(doc.data()));

  return data.reverse(); // 시간순 정렬 (오래된 것부터)
};
