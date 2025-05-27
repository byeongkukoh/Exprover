import { db } from './firebase';
import {
  collection,
  query,
  orderBy,
  limit,
  getDocs
} from 'firebase/firestore';

export const fetchSensorData = async (sensor) => {
  try {
    const q = query(
      collection(db, sensor),
      orderBy('time', 'desc'),
      limit(1)
    );
    const snapshot = await getDocs(q);
    let latestData = null;
    snapshot.forEach((doc) => {
      latestData = doc.data();
    });
    return latestData;
  } catch (error) {
    console.error(`Failed to fetch data from ${sensor}:`, error);
    return null;
  }
};
